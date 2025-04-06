#include "ADS1247.h"
#include "spi.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

/**
 * @brief 复位ads1247
 *
 * @param handle ads1247句柄
 * @return ADS1247_Staus_t 操作状态
 */
ADS1247_Staus_t ADS1247_Reset(ADS1247_Handle_t *handle)
{
	if (handle == NULL || *handle == NULL)
		return ADS1247_ERROR;
	HAL_GPIO_WritePin((*handle)->conf.ReSet.port, (*handle)->conf.ReSet.pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin((*handle)->conf.ReSet.port, (*handle)->conf.ReSet.pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin((*handle)->conf.ReSet.port, (*handle)->conf.ReSet.pin, GPIO_PIN_SET);
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
ADS1247_Staus_t ADS1247_Writebyte(ADS1247_Handle_t *handle, uint8_t *data, size_t size)
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
ADS1247_Staus_t ADS1247_Readbyte(ADS1247_Handle_t *handle, uint8_t *data, size_t size)
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
ADS1247_Staus_t ADS1247_WriteCommand(ADS1247_Handle_t *handle, ADS1247_Command_t cmd)
{

	uint8_t data = (uint8_t)cmd;
	return ADS1247_Writebyte(handle, &data, 1);
}

/**
 * @brief 写入单个寄存器
 *
 * @param handle ads1247句柄
 * @param reg 寄存器 ADS1247_Reg_t 在此枚举
 * @param conf 配置字节
 * @return ADS1247_Staus_t 操作状态
 */
ADS1247_Staus_t ADS1247_WriteReg(ADS1247_Handle_t *handle, ADS1247_Reg_t reg, uint8_t conf)
{

	uint8_t data[3] = {(uint8_t)(WREG_CMD | reg), 0x00, conf};
	// printf("{ADS_WREG : \"%x\" , \"data\" : %x}\n",reg,data[2]);

	return ADS1247_Writebyte(handle, data, 3);
}

/**
 * @brief 读取单个寄存器
 *
 * @param reg 读哪个寄存器
 * @param buffer 读取寄存器的buffer
 * @return ADS1247_Staus_t 操作状态
 */
ADS1247_Staus_t ADS1247_ReadReg(ADS1247_Handle_t *handle, ADS1247_Reg_t reg, uint8_t *buffer)
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
 * @brief ADS1247初始化，句柄必须通过初始化后才有效
 *
 * @param handle ADS1247句柄
 * @param conf 配置结构体
 * @return ADS1247_Staus_t 操作状态
 */
ADS1247_Staus_t ADS1247_CreateDevice(ADS1247_Handle_t *handle, ADS1247_Config_t *conf)
{
	if (handle == NULL || *handle != NULL)
		return ADS1247_ERROR;

	*handle = calloc(1, sizeof(ADS1247_Class_t));
	if (*handle == NULL)
		return ADS1247_ERROR;

	(*handle)->conf = *conf;
	HAL_GPIO_WritePin((*handle)->conf.ReSet.port, (*handle)->conf.ReSet.pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin((*handle)->conf.Start.port, (*handle)->conf.Start.pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin((*handle)->conf.CS.port, (*handle)->conf.CS.pin, GPIO_PIN_SET);
	HAL_Delay(100);

	ADS1247_Reset(handle);
	HAL_Delay(10);

	HAL_GPIO_WritePin((*handle)->conf.Start.port, (*handle)->conf.Start.pin, GPIO_PIN_SET); // 拉高START确保能配置寄存器
	HAL_GPIO_WritePin((*handle)->conf.CS.port, (*handle)->conf.CS.pin, GPIO_PIN_RESET);		// 拉高START确保能配置寄存器
	ADS1247_Staus_t ret = ADS1247_WriteCommand(handle, WAKEUP_CMD);

	ret = ADS1247_WriteReg(handle, MUX0_REG, MUX_SP_AIN1 | MUX_SN_AIN2 | BSC_OFF); // 配置采样通道和烧毁电流源 0b00001010

	ret = ADS1247_WriteReg(handle, VBIAS_REG, VBIAS_AIN0_DISABLE | VBIAS_AIN1_DISABLE | VBIAS_AIN2_DISABLE | VBIAS_AIN3_DISABLE); // 配置偏置电压 0b00000000

	ret = ADS1247_WriteReg(handle, MUX1_REG, NORMAL_MODEL | REFSELT_REFP0_REFN0 | INTERNAL_ALWAYS_ON); // 选择外部参考，运行在正常模式，内部参考打开 0b00100000

	ret = ADS1247_WriteReg(handle, SYS0_REG, DR_5SPS | PGA_1X); // 配置采样速率和PGA 0b00000110

	ret = ADS1247_WriteReg(handle, IDAC0_REG, EXCITA_CURRENT_500uA | DRDY_MODE_DOUT_ONLY); // 配置激励电流为500ua 以及DRDY行为 00000100

	ret = ADS1247_WriteReg(handle, IDAC1_REG, (IDAC1_OUTPUT1_AIN0 | IDAC1_OUTPUT2_AIN3)); // 选择 AIN0 AIN3输出激励电流 0b11001111

	ret = ADS1247_WriteReg(handle, GPIOCFG_REG, GPIO0_DISABLE | GPIO1_DISABLE | GPIO2_DISABLE | GPIO3_DISABLE); // 禁用GPIO 0b00000000

	ret = ADS1247_WriteReg(handle, GPIODIR_REG, 0x00);

	ret = ADS1247_WriteReg(handle, GPIODAT_REG, 0x00);

	ret = ADS1247_WriteCommand(handle, RDATAC_CMD);

	return ret;
}

/**
 * @brief 回调函数，请放在stm32xx_it.c 文件中的外部回调处。中断方式为下降沿中断
 *
 * @param handle 句柄
 * @return ADS1247_Staus_t 操作状态
 */
ADS1247_Staus_t ADS1247_Callback(ADS1247_Handle_t *handle)
{
	if (handle == NULL || *handle == NULL)
		return ADS1247_ERROR;
	(*handle)->flag.DRDY = ADS1247_Flag_ON;
	return ADS1247_OK;
}

/**
 * @brief 获取ADC转换结果
 *
 * @param handle ads1247句柄
 * @param buffer 读取结果缓冲区
 * @return ADS1247_Staus_t 操作状态，只有函数返回ADS1247_OK下的值有效
 */
ADS1247_Staus_t ADS1247_getADC(ADS1247_Handle_t *handle, uint32_t *buffer)
{
	if (handle == NULL || *handle == NULL)
		return ADS1247_ERROR;
	if ((*handle)->flag.DRDY == ADS1247_Flag_OFF)
		return ADS1247_DRDYERROR;
	(*handle)->flag.DRDY = ADS1247_Flag_OFF;

	uint8_t data[3] = {0};

	ADS1247_Staus_t ret = ADS1247_Readbyte(handle, data, 3);
	if (ret != ADS1247_OK)
		return ret;

	*buffer = (uint32_t)(data[0] << 16) | (uint16_t)(data[1] << 8) | data[2];
	return ADS1247_OK;
}

/**
 * @brief 设置采样速率和pga
 *
 * @param handle 句柄
 * @param dataRate 数据速率
 * @param pgaGain PGA
 * @return ADS1247_Staus_t 操作状态
 */
ADS1247_Staus_t ADS1247_SetDataRateAndPGA(ADS1247_Handle_t *handle, ADS1247_SampleRate_t dataRate, ADS1247_PGA_Gain_t pgaGain)
{
	if (handle == NULL || *handle == NULL)
		return ADS1247_ERROR;
	return ADS1247_WriteReg(handle, SYS0_REG, (uint8_t)dataRate | (uint8_t)pgaGain);
}

/**
 * @brief 删除ADS1247
 *
 * @param handle 句柄
 * @return ADS1247_Staus_t 操作状态
 */
ADS1247_Staus_t ADS1247_Delete(ADS1247_Handle_t *handle)
{
	if (handle == NULL || *handle == NULL)
		return ADS1247_ERROR;

	free(*handle);
	*handle = NULL;

	return ADS1247_OK;
}

/**
 * @brief 配置偏置电压
 * 
 * @param handle 句柄
 * @param VBIAS_AIN0 
 * @param VBIAS_AIN1 
 * @param VBIAS_AIN2 
 * @param VBIAS_AIN3 
 * @return ADS1247_Staus_t 
 */
ADS1247_Staus_t ADS1247_ConfigureVBIAS(ADS1247_Handle_t *handle,uint8_t VBIAS_AIN0,uint8_t VBIAS_AIN1,uint8_t VBIAS_AIN2,uint8_t VBIAS_AIN3)
{
	if(handle == NULL || *handle == NULL) return ADS1247_ERROR;
	return ADS1247_WriteReg(handle,VBIAS_REG,VBIAS_AIN0 | VBIAS_AIN1 | VBIAS_AIN2 | VBIAS_AIN3);
}
