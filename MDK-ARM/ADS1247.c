#include "ADS1247.h"
#include "spi.h"

volatile ADS1247_Class_t ads1247 = {0}; // ads1247 实例

#ifndef ON
#define ON (1)
#endif

#ifndef OFF
#define OFF (0)
#endif

/**
 * @brief 复位ADS1247
 * 
 */
void ADS1247_Reset()
{
    ADS1247_RESET_SET;
	ADS1247_RESET_CLR;
    HAL_Delay(10);
	ADS1247_RESET_SET;
}

/**
 * @brief 写字节数据
 * 
 * @param data 要写入的字节
 * @param size 写入字节大小
 * @return spi传输成功 : 0x00 spi传输失败 : 0xff  
 */
uint8_t ADS1247_Writebyte(uint8_t *data,size_t size)
{
    if(HAL_SPI_Transmit(ADS1247_SPI_Handle,data,size,ADS1247_TIMEOUT) != HAL_OK) 
    return 0xff;
    return 0x00;
    
}

/**
 * @brief 读字节数据
 * 
 * @param data 读取数据的缓存区
 * @param size 读取数据的大小
 * @return uint8_t spi通信成功返回 0x00 通信失败返回 0xff
 */
uint8_t ADS1247_Readbyte(uint8_t *data,size_t size)
{
    if(HAL_SPI_Receive(ADS1247_SPI_Handle,data,size,ADS1247_TIMEOUT) != HAL_OK) 
    return 0xff;
    return 0x00;
}

/**
 * @brief ADS1247 写命令
 * 
 * @param cmd 要写入的命令，命令已经在 ADS1247_Command_t中枚举 
 * @return 命令发送成功 : 0x00 , 命令发送失败 : 0xff
 */
uint8_t ADS1247_WriteCommand(ADS1247_Command_t cmd)
{
    uint8_t data = (uint8_t)cmd;
    return ADS1247_Writebyte(&data,1);
}


/**
 * @brief 写寄存器配置函数,单字节写入配置
 * 
 * @param reg 要操作的寄存器，所有寄存器已在 ADS1247_Reg_t 类型中枚举
 * @param conf 要写入的配置，请对应配置文件选择合适的配置
 * @return uint8_t 写入成功返回0x00 写入失败返回 0xff
 */
uint8_t ADS1247_WriteReg(ADS1247_Reg_t reg,uint8_t conf)
{
    uint8_t data[3] = { (WREG_CMD | reg),0x00,conf};
    return ADS1247_Writebyte(data,3);

}

/**
 * @brief 读取单个寄存器
 * 
 * @param reg 读哪个寄存器
 * @return uint8_t 读取到的数据， 通信失败返回0xff
 */
uint8_t ADS1247_ReadReg(ADS1247_Reg_t reg)
{
    uint8_t data[3] = { (RREG_CMD | reg),0x00,0x00};
    uint8_t ret = ADS1247_Writebyte(data,2);
    ret = ADS1247_Readbyte(&data[2],1);
    if(ret != 0x00) 
    return ret;
    return data[2];
}

/**
 * @brief ADS1247初始化
 * 
 */
void ADS1247_Init()
{
	ADS1247_Reset(); // 复位
	ADS1247_CS_CLR; // 拉低CS
	ADS1247_WriteCommand(WAKEUP_CMD); // 发送唤醒命令
	ADS1247_WriteReg(MUX0_REG,MUX_SP_AIN0 | MUX_SN_AIN1 | MUX_SP_AIN2 | MUX_SN_AIN3 | BSC_OFF); // 配置模拟输入和电流源
	ADS1247_WriteReg(VBIAS_REG,VBIAS_AIN0_DISABLE); // 配置偏置电压
    ADS1247_WriteReg(MUX1_REG,NORMAL_MODEL | REFSELT_REFP0_REFN0 | INTERNAL_ALWAYS_OFF); // 选择工作模式和参考电压
    ADS1247_SetDataRateAndPGA(SAMPLE_RATE_320SPS,PGA_GAIN_1X);
	ADS1247_WriteReg(IDAC0_REG,EXCITA_CURRENT_OFF | DRDY_MODE_DOUT_ONLY); // 设置采样速率和PGA
    ADS1247_WriteReg(IDAC1_REG,IDAC1_OUTPUT2_AIN0 | IDAC1_OUTPUT1_AIN2); // 选择激励电流输出的引脚
    ADS1247_WriteReg(GPIOCFG_REG,GPIO0_DISABLE | GPIO1_DISABLE | GPIO2_DISABLE | GPIO3_DISABLE); // 配置GPIO
	ADS1247_START_SET; // 开始连续转换
 
}

/**
 * @brief ADS1247中断回调函数
 * @note 在stm32xx_it.c中选择DRDY引脚对应的中断号调用
 * @note 下降沿触发中断,此函数会记录转换的次数，DRDY会告诉你转换还剩多少没有被读取
 * 
 */
void ADS1247_Callback()
{
	   ads1247.flag.DRDY ++;
}

/**
 * @brief 获取ADS1247转换的ADC
 * @note 只有在DRDY 不为0的时候读取转换结果，否则返回错误码0xffffffff
 *
 * @return 由于只有在DRDY中有转换完成记录的情况下才返回转换结果
 *         请把函数返回值赋值到一个变量里然后判断这个变量是不是错误码
 */
uint32_t ADS1247_getADC()
{
	if(ads1247.flag.DRDY == 0) return 0xFFFFFFFF;
	ads1247.flag.DRDY --; // 自减DRDY
	uint8_t data[3] = {0};
	HAL_SPI_Receive(ADS1247_SPI_Handle,data,3,ADS1247_TIMEOUT);
	return  (uint32_t)(data[0] << 16) | (uint16_t)(data[1] << 8) | data[0];
}


/**
 * @brief 设置数据采样速率和 PGA 增益
 * 
 * @param dataRate 数据采样速率 所有速度已在 ADS1247_SampleRate_t 中枚举
 * @param pgaGain PGA 增益 所有PGA增益已在 ADS1247_PGA_Gain_t 中枚举
 * @note 在初始化之后调用有效
 * 
 * @return 返回操作状态，0x00 为成功，0xff表示失败
 */
uint8_t ADS1247_SetDataRateAndPGA(ADS1247_SampleRate_t dataRate, ADS1247_PGA_Gain_t pgaGain)
{
	ADS1247_START_CLR; // 先停止转换
    ads1247.conf.DR = dataRate;
    ads1247.conf.PGA = pgaGain;
	uint8_t conf = (uint8_t)dataRate | (uint8_t) pgaGain;
	uint8_t ret = ADS1247_WriteReg(SYS0_REG,conf); // 发送配置
	ADS1247_START_SET; // 启动转换
	return ret;
}

