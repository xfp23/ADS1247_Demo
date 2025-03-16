#include "ADS1247.h"
#include "spi.h"

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
 * @return uint8_t spi通信成功返回0x00 通信失败返回0xff
 */
uint8_t ADS1247_ReadReg(ADS1247_Reg_t reg)
{
    uint8_t data[3] = { (RREG_CMD | reg),0x00,0x00};
    uint8_t ret = ADS1247_Writebyte(data,2);
    uint8_t ret = ADS1247_Readbyte(&data[2],1);
    return ret;
}