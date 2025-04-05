#include "ADS1247.h"
#include "spi.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

volatile ADS1247_Class_t ads1247 = {0}; // ads1247 实例

// uint8_t regValue[3] = {0};
#ifndef ON
#define ON (1)
#endif

#ifndef OFF
#define OFF (0)
#endif

/**
 * @brief 复位ads1247
 * 
 * @param handle ads1247句柄
 * @return ADS1247_Staus_t 操作状态
 */
ADS1247_Staus_t ADS1247_Reset(ADS1247_Handle *handle)
{
	if(handle == NULL || *handle == NULL) return ADS1247_ERROR;
	HAL_GPIO_WritePin((*handle)->conf.ReSet.port,(*handle)->conf.ReSet.pin,GPIO_PIN_SET);
	HAL_Delay(10);
    HAL_GPIO_WritePin((*handle)->conf.ReSet.port,(*handle)->conf.ReSet.pin,GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin((*handle)->conf.ReSet.port,(*handle)->conf.ReSet.pin,GPIO_PIN_SET);
	return ADS1247_OK;
}

/**
 * @brief ADS1247发送数据API
 * 
 * @param handle ads1247句柄
 * @param data 要写入的数据
 * @param size 要写入的数据大小 单位: 字节
 * @return ADS1247_Staus_t 操作状态
 */
ADS1247_Staus_t ADS1247_Writebyte(ADS1247_Handle *handle, uint8_t *data, size_t size)
{
	if (handle == NULL || *handle == NULL)
		return ADS1247_ERROR; // 返回错误
	HAL_GPIO_WritePin((*handle)->conf.CS.port, (*handle)->conf.CS.pin, GPIO_PIN_RESET);
#if ADS1120_USEDMA
	if ((*handle)->flag.DMA == ADS1247_Flag_ON)
	{
		if (HAL_SPI_Transmit_DMA((*handle)->conf.spi, data, size) != HAL_OK)
		{
			HAL_GPIO_WritePin((*handle)->conf.CS.port, (*handle)->conf.CS.pin, GPIO_PIN_SET);
			return ADS1247_SPIERROR;
		}
		HAL_GPIO_WritePin((*handle)->conf.CS.port, (*handle)->conf.CS.pin, GPIO_PIN_SET);
		return ADS1247_OK;
	}
#endif
	if (HAL_SPI_Transmit((*handle)->conf.spi, data, size, (*handle)->conf.Timeout) != HAL_OK)
	{
		HAL_GPIO_WritePin((*handle)->conf.CS.port, (*handle)->conf.CS.pin, GPIO_PIN_SET);
		return ADS1247_SPIERROR;
	}
	HAL_GPIO_WritePin((*handle)->conf.CS.port, (*handle)->conf.CS.pin, GPIO_PIN_SET);
	return ADS1247_OK;
}

/**
 * @brief ads1247读字节API
 * 
 * @param handle ads1247句柄
 * @param data 读取数据缓冲区
 * @param size 读取数据大小 单位 : 字节
 * @return ADS1247_Staus_t 操作状态
 */
ADS1247_Staus_t ADS1247_Readbyte(ADS1247_Handle *handle, uint8_t *data, size_t size)
{
	if (handle == NULL || *handle == NULL)
		return ADS1247_ERROR; // 返回错误
	HAL_GPIO_WritePin((*handle)->conf.CS.port, (*handle)->conf.CS.pin, GPIO_PIN_RESET);
#if ADS1120_USEDMA
	if ((*handle)->flag.DMA == ADS1247_Flag_ON)
	{
		if (HAL_SPI_Receive_DMA((*handle)->conf.spi, data, size) != HAL_OK)
		{
			HAL_GPIO_WritePin((*handle)->conf.CS.port, (*handle)->conf.CS.pin, GPIO_PIN_SET);
			return ADS1247_SPIERROR;
		}
		HAL_GPIO_WritePin((*handle)->conf.CS.port, (*handle)->conf.CS.pin, GPIO_PIN_SET);
		return ADS1247_OK;
	}
#endif
	if (HAL_SPI_Receive((*handle)->conf.spi, data, size, (*handle)->conf.Timeout) != HAL_OK)
	{
		HAL_GPIO_WritePin((*handle)->conf.CS.port, (*handle)->conf.CS.pin, GPIO_PIN_SET);
		return ADS1247_SPIERROR;
	}
	HAL_GPIO_WritePin((*handle)->conf.CS.port, (*handle)->conf.CS.pin, GPIO_PIN_SET);
	return ADS1247_OK;
}

/**
 * @brief ads1247写入命令
 * 
 * @param handle ads1247句柄
 * @param cmd 命令 ADS1247_Command_t 在此枚举
 * @return ADS1247_Staus_t 操作状态
 */
ADS1247_Staus_t ADS1247_WriteCommand(ADS1247_Handle *handle,ADS1247_Command_t cmd)
{

    uint8_t data = (uint8_t)cmd;
    return ADS1247_Writebyte(handle,&data, 1);

}

/**
 * @brief 写入单个寄存器
 * 
 * @param handle ads1247句柄
 * @param reg 寄存器 ADS1247_Reg_t 在此枚举
 * @param conf 配置字节
 * @return ADS1247_Staus_t 操作状态
 */
ADS1247_Staus_t ADS1247_WriteReg(ADS1247_Handle *handle,ADS1247_Reg_t reg, uint8_t conf)
{

    uint8_t data[3] = { (uint8_t)(WREG_CMD | reg), 0x00, conf};
	// printf("{ADS_WREG : \"%x\" , \"data\" : %x}\n",reg,data[2]);
	
   return ADS1247_Writebyte(handle,data,3);
}


/**
 * @brief 读取单个寄存器
 *
 * @param reg 读哪个寄存器
 * @param buffer 读取寄存器的buffer
 * @return ADS1247_Staus_t 操作状态
 */
ADS1247_Staus_t ADS1247_ReadReg(ADS1247_Handle *handle, ADS1247_Reg_t reg, uint8_t *buffer)
{
	uint8_t data[2] = {(RREG_CMD | reg), 0x00};
	ADS1247_Staus_t ret = ADS1247_Writebyte(handle, data, 2);
	if (ret != ADS1247_OK)
		return ret; // 返回异常
	ret = ADS1247_Readbyte(handle, buffer, 1);
	// printf("{ADS_RREG : \"%x\" , \"data\" : %x}\n",reg,data[2]);
	return ret;

}

/**
 * @brief ADS1247初始化
 *
 */
ADS1247_Staus_t ADS1247_Init(ADS1247_Handle *handle,ADS1247_Config_t *conf)
{
	if (handle == NULL || *handle != NULL) return ADS1247_ERROR; // 返回错误

	handle = calloc(1, sizeof(ADS1247_Class_t)); // 分配堆内存
	if(*handle == NULL) return ADS1247_ERROR; // 返回错误

	
	HAL_GPIO_WritePin((*handle)->conf.ReSet.port,(*handle)->conf.ReSet.pin,GPIO_PIN_SET); // 确保复位引脚是高电平
	HAL_GPIO_WritePin((*handle)->conf.CS.port,(*handle)->conf.CS.pin,GPIO_PIN_SET); // 确保START引脚是低电平
	HAL_GPIO_WritePin((*handle)->conf.CS.port,(*handle)->conf.CS.pin,GPIO_PIN_SET);    // 确保CS是高电平
	HAL_Delay(100);
	
	ADS1247_Reset(); // 复位ADS1247 使用REST引脚硬件复位，不适用命令复位
	HAL_Delay(10);   // 至少0.6ms
	
	ADS1247_START_SET; // 拉高START确保能配置寄存器
	ADS1247_RESET_SET;
	ADS1247_CS_CLR;    // 拉低CS使能spi通信
//	HAL_Delay(INIT_DELAYTIME);
	ADS1247_WriteCommand(WAKEUP_CMD);
//	HAL_Delay(INIT_DELAYTIME);
//	ADS1247_WriteCommand(SDATAC_CMD);
//	HAL_Delay(10);
	
	ADS1247_WriteReg(MUX0_REG,MUX_SP_AIN1 | MUX_SN_AIN2 | BSC_OFF); // 配置采样通道和烧毁电流源 0b00001010
	HAL_Delay(INIT_DELAYTIME);
	
	ADS1247_WriteReg(VBIAS_REG,VBIAS_AIN0_DISABLE | VBIAS_AIN1_DISABLE | VBIAS_AIN2_DISABLE | VBIAS_AIN3_DISABLE); // 配置偏置电压 0b00000000
	HAL_Delay(INIT_DELAYTIME);
	
	ADS1247_WriteReg(MUX1_REG,NORMAL_MODEL | REFSELT_REFP0_REFN0 | INTERNAL_ALWAYS_ON); // 选择外部参考，运行在正常模式，内部参考打开 0b00100000
	HAL_Delay(INIT_DELAYTIME);
//	ADS1247_START_SET;
	ADS1247_WriteReg(SYS0_REG,RATE | PGA); // 配置采样速率和PGA 0b00000110
	HAL_Delay(INIT_DELAYTIME);
	
	ADS1247_WriteReg(IDAC0_REG, EXCITA_CURRENT_500uA | DRDY_MODE_DOUT_ONLY); // 配置激励电流为500ua 以及DRDY行为 00000100
	HAL_Delay(INIT_DELAYTIME);
	
	ADS1247_WriteReg(IDAC1_REG, (IDAC1_OUTPUT1_AIN0 | IDAC1_OUTPUT2_AIN3)); // 选择 AIN0 AIN3输出激励电流 0b11001111
	HAL_Delay(INIT_DELAYTIME);
	
	ADS1247_WriteReg(GPIOCFG_REG,GPIO0_DISABLE | GPIO1_DISABLE | GPIO2_DISABLE | GPIO3_DISABLE ); // 禁用GPIO 0b00000000
	HAL_Delay(INIT_DELAYTIME);
	
	ADS1247_WriteReg(GPIODIR_REG,0x00); // 写入默认值
	HAL_Delay(INIT_DELAYTIME);
	ADS1247_WriteReg(GPIODAT_REG,0x00); // 写入默认值

	ADS1247_WriteCommand(RDATAC_CMD);

}




/**
 * @brief ADS1247中断回调函数
 * @note 在stm32xx_it.c中选择DRDY引脚对应的中断号调用
 * @note 下降沿触发中断,此函数会记录转换的次数，DRDY会告诉你转换还剩多少没有被读取
 *
 */
void ADS1247_Callback()
{
    ads1247.flag.DRDY = ON;
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
    if (ads1247.flag.DRDY == OFF)
        return 0xFFFFFFFF;
    ads1247.flag.DRDY = OFF; 
	
//    ADS1247_WriteReg(IDAC0_REG, EXCITA_CURRENT_500uA | DRDY_MODE_DOUT_ONLY);  // 设定 500μA 激励电流
//	ADS1247_WriteReg(IDAC1_REG,0x03); // 选择激励电流输出引脚为AIN0 AIN3
    uint8_t data[3] = {0};
	if(HAL_SPI_Receive(ADS1247_SPI_Handle, data, 3, ADS1247_TIMEOUT) != HAL_OK)
	{
	HAL_Delay(1);
	}
    
    return (uint32_t)(data[0] << 16) | (uint16_t)(data[1] << 8) | data[0];
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
    uint8_t conf = (uint8_t)dataRate | (uint8_t)pgaGain;
    uint8_t ret = ADS1247_WriteReg(SYS0_REG, conf); // 发送配置
    ADS1247_START_SET;                              // 启动转换
    return ret;
}
