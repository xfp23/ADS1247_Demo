#include "ADS1247.h"
#include "spi.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

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
static ADS1247_Staus_t ADS1247_Writebyte(ADS1247_Handle_t *handle, uint8_t *data, size_t size)
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
static ADS1247_Staus_t ADS1247_Readbyte(ADS1247_Handle_t *handle, uint8_t *data, size_t size)
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

	ret = ADS1247_SetMuxAndBSC(handle, ADS1247_MUX_AIN1, ADS1247_MUX_AIN2, ADS1247_BSC_OFF);

	ret = ADS1247_ConfigureVBIAS(handle, ADS1247_VBIAS_DISABLE, ADS1247_VBIAS_DISABLE, ADS1247_VBIAS_DISABLE, ADS1247_VBIAS_DISABLE); // 配置偏置电压 0b00000000

	ret = ADS1247_SetReference(handle, ADS1247_REFP0_REFN0, ADS1247_ALWAYS_ON);

	ret = ADS1247_SetDataRateAndPGA(handle, SAMPLE_RATE_5SPS, PGA_GAIN_1X);

	ret = ADS1247_SetIDACWithDRDYMode(handle, ADS1247_EXCITA_CURRENT_OFF, ADS1247_DRDY_ONLY);

	ret = ADS1247_SetIDACOutputPin(handle, ADS1247_IDAC_OUTPUT_AIN0, ADS1247_IDAC_OUTPUT_AIN0);

	ret = ADS1247_WriteReg(handle, GPIOCFG_REG, 0x00); // 禁用GPIO 0b00000000

	ret = ADS1247_WriteReg(handle, GPIODIR_REG, 0x00);

	ret = ADS1247_WriteReg(handle, GPIODAT_REG, 0x00);

	ret = ADS1247_WriteCommand(handle, RDATAC_CMD);

	return ret;
}

ADS1247_Staus_t ADS1247_SetIDACOutputPin(ADS1247_Handle_t *handle, ADS1247_IDACOutputDIR_t Output1, ADS1247_IDACOutputDIR_t Output2)
{
	if (handle == NULL || *handle == NULL)
		return ADS1247_ERROR;
	return ADS1247_WriteReg(handle, IDAC1_REG, (uint8_t)((Output1 << 4) | (Output2)));
}

ADS1247_Staus_t ADS1247_Callback(ADS1247_Handle_t *handle)
{
	if (handle == NULL || *handle == NULL)
		return ADS1247_ERROR;
	(*handle)->flag.DRDY = ADS1247_Flag_ON;
	return ADS1247_OK;
}

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

ADS1247_Staus_t ADS1247_SetDataRateAndPGA(ADS1247_Handle_t *handle, ADS1247_SampleRate_t dataRate, ADS1247_PGA_Gain_t pgaGain)
{
	if (handle == NULL || *handle == NULL)
		return ADS1247_ERROR;
	return ADS1247_WriteReg(handle, SYS0_REG, (uint8_t)dataRate | (uint8_t)(pgaGain << 4));
}

ADS1247_Staus_t ADS1247_Delete(ADS1247_Handle_t *handle)
{
	if (handle == NULL || *handle == NULL)
		return ADS1247_ERROR;

	free(*handle);
	*handle = NULL;

	return ADS1247_OK;
}

ADS1247_Staus_t ADS1247_ConfigureVBIAS(ADS1247_Handle_t *handle, ADS1247_VBIASAINx_t VBIAS_AIN0, ADS1247_VBIASAINx_t VBIAS_AIN1, ADS1247_VBIASAINx_t VBIAS_AIN2, ADS1247_VBIASAINx_t VBIAS_AIN3)
{
	if (handle == NULL || *handle == NULL)
		return ADS1247_ERROR;
	return ADS1247_WriteReg(handle, VBIAS_REG, VBIAS_AIN0 | (VBIAS_AIN1 << 1) | (VBIAS_AIN2 << 2) | (VBIAS_AIN3 << 3));
}

ADS1247_Staus_t ADS1247_SetIDACWithDRDYMode(ADS1247_Handle_t *handle, ADS1247_EXCITACurrent_t IDAC, ADS1247_DRDYMode_t DRDY_Mode)
{
	if (handle == NULL || *handle == NULL)
		return ADS1247_ERROR;
	return ADS1247_WriteReg(handle, IDAC0_REG, IDAC | DRDY_Mode << 3);
}

ADS1247_Staus_t ADS1247_SetMuxAndBSC(ADS1247_Handle_t *handle, ADS1247_Mux_t Mux_SP, ADS1247_Mux_t Mux_SN, ADS1247_BSC_t BSC)
{
	if (handle == NULL || *handle == NULL)
		return ADS1247_ERROR;
	return ADS1247_WriteReg(handle, MUX0_REG, (uint8_t)(Mux_SP << 3) | (uint8_t)Mux_SN | (uint8_t)(BSC << 6));
}

ADS1247_Staus_t ADS1247_SetReference(ADS1247_Handle_t *handle, ADS1247_VREF_t vref, ADS1247_Internal_t internal)
{
	if (handle == NULL || *handle == NULL)
		return ADS1247_ERROR;
	return ADS1247_WriteReg(handle, MUX1_REG, 0x00 | (uint8_t)(vref << 3) | (uint8_t)(internal << 5));
}

ADS1247_Staus_t ADS1247_GPIO_WritePin(ADS1247_Handle_t *handle, ADS1247_Pin_t pin, ADS1247_GPIOData_t data)
{
	if (handle == NULL || *handle == NULL)
		return ADS1247_ERROR;
	uint8_t Reg_data = 0;
	ADS1247_Staus_t ret = ADS1247_ReadReg(handle, GPIODAT_REG, &Reg_data);
	if (ret != ADS1247_OK)
		return ret;
	Reg_data &= ~(1 << pin);
	Reg_data |= (data << pin);
	return ADS1247_WriteReg(handle, GPIODAT_REG, Reg_data);
}

ADS1247_Staus_t ADS1247_GPIO_Config(ADS1247_Handle_t *handle, ADS1247_Pin_t pin, ADS1247_GPIODIR_t DIR)
{
	if (handle == NULL || *handle == NULL)
		return ADS1247_ERROR;
	uint8_t Reg_data = 0;
	ADS1247_Staus_t ret = ADS1247_ReadReg(handle, GPIOCFG_REG, &Reg_data);
	if (ret != ADS1247_OK)
		return ret;
	Reg_data |= 1 << pin;
	ret = ADS1247_WriteReg(handle, GPIOCFG_REG, Reg_data);
	if (ret != ADS1247_OK)
		return ret;
	ret = ADS1247_ReadReg(handle, GPIODIR_REG, &Reg_data);
	if (ret != ADS1247_OK)
		return ret;
	Reg_data &= ~(1 << pin);
	Reg_data |= (DIR << pin);
	return ADS1247_WriteReg(handle, GPIODIR_REG, Reg_data);
}

ADS1247_Staus_t ADS1247_GPIO_Reset(ADS1247_Handle_t *handle, ADS1247_Pin_t pin)
{
	if (handle == NULL || *handle == NULL)
		return ADS1247_ERROR;
	ADS1247_Staus_t ret = ADS1247_OK;
	if (pin == ADS1247_GPIOALL)
	{
		ret = ADS1247_WriteReg(handle, GPIOCFG_REG, 0x00);
		ret = ADS1247_WriteReg(handle, GPIODIR_REG, 0x00);
		ret = ADS1247_WriteReg(handle, GPIODAT_REG, 0x00);
		return ret;
	}
	uint8_t Reg_data = 0x00;
	ret = ADS1247_ReadReg(handle, GPIOCFG_REG, &Reg_data);
	if (ret != ADS1247_OK)
		return ret;
	Reg_data &= ~(1 << pin);
	ret = ADS1247_WriteReg(handle, GPIOCFG_REG, Reg_data);
	if (ret != ADS1247_OK)
		return ret;

	ret = ADS1247_ReadReg(handle, GPIODIR_REG, &Reg_data);
	if (ret != ADS1247_OK)
		return ret;
	Reg_data &= ~(1 << pin);
	ret = ADS1247_WriteReg(handle, GPIODIR_REG, Reg_data);
	if (ret != ADS1247_OK)
		return ret;

	ret = ADS1247_ReadReg(handle, GPIODAT_REG, &Reg_data);
	if (ret != ADS1247_OK)
		return ret;
	Reg_data &= ~(1 << pin);
	ret = ADS1247_WriteReg(handle, GPIODAT_REG, Reg_data);
	if (ret != ADS1247_OK)
		return ret;
	return ADS1247_OK;
}
