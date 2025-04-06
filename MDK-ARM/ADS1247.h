#ifndef ADS1247_H
#define ADS1247_H

#pragma anon_unions

/**
 * @file ADS1247.h
 * @author xfp23
 * @brief 此文件是ADS1247的驱动头文件
 * @version 0.1
 * @date 2025-03-16
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "main.h"
#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define ADS1120_USEDMA 0 // 不启用DMA编译逻辑

// 寄存器枚举
// Table 29. ADS1247 and ADS1248 Register Map
typedef enum
{
    MUX0_REG = 0x00,    // mux 复用器0
    VBIAS_REG = 0x01,   // 偏置电压
    MUX1_REG = 0x02,    // mux复用器1
    SYS0_REG = 0x03,    // 系统设置PGA与采样速率寄存器
    OFC0_REG = 0x04,    // 系统偏移校准
    OFC1_REG = 0x05,    // 系统偏移校准
    OFC2_REG = 0x06,    // 系统偏移校准
    FSC0_REG = 0x07,    // 系统增益校准
    FSC1_REG = 0x08,    // 系统增益校准
    FSC2_REG = 0x09,    // 系统增益校准
    IDAC0_REG = 0x0A,   // 激励电流大小
    IDAC1_REG = 0x0B,   // 激励电流方向
    GPIOCFG_REG = 0x0C, // GPIO配置
    GPIODIR_REG = 0x0D, // GPIO方向
    GPIODAT_REG = 0x0E, // GPIO数据

} ADS1247_Reg_t;

// 当 START 引脚为低电平或设备处于掉电模式时，只能发出 RDATA、RDATAC、SDATAC、WAKEUP 和 NOP 命令。
typedef enum
{
    WAKEUP_CMD = 0x00,      // 唤醒命令
    SLEEP_CMD = 0x02,       // 睡眠
    SYNC_CMD = 0x04,        // 转换命令
    RESET_CMD = 0x06,       // 复位命令
    NOP_CMD = 0xFF,         // 空命令
    RDATA_CMD = 0x12,       // 读一次数据
    RDATAC_CMD = 0x14,      // 连续读取
    SDATAC_CMD = 0x16,      // 停止连续转换
    RREG_CMD = (0x02 << 4), // 读寄存器 0000 nnnn
    WREG_CMD = (0x04 << 4), // 写寄存器 0000 nnnn
    SYSOCAL_CMD = 0x60,     // 系统偏移校准
    SYSGCAL_CMD = 0x61,     // 系统增益校准
    SELFOCAL_CMD = 0x62,    // 系统自校准
} ADS1247_Command_t;

typedef enum
{
    ADS1247_OK,
    ADS1247_SPIERROR, // spi通信错误
    ADS1247_ERROR,    // 错误
    ADS1247_DRDYERROR,
} ADS1247_Staus_t;

typedef enum
{
    ADS1247_Flag_OFF = 0x00, // 关
    ADS1247_Flag_ON = 0x01,  // 开
} ADS1247_FlagSwitch_t;

// mux0配置

typedef enum
{
    ADS1247_MUX_AIN0,
    ADS1247_MUX_AIN1,
    ADS1247_MUX_AIN2,
    ADS1247_MUX_AIN3,
} ADS1247_Mux_t;

typedef enum
{
    ADS1247_BSC_OFF,
    ADS1247_BSC_0_5UA,
    ADS1247_BSC_2UA,
    ADS1247_BSC_10UA
} ADS1247_BSC_t; // 烧毁电流源选项

// VBIAS 偏置电压配置
typedef enum
{
    ADS1247_VBIAS_DISABLE, // 禁用 AIN0 的偏置电压
    ADS1247VBIAS_ENABLE,   // 将中间电源(AVDD + AVSSE) / 2 的偏置电压施加到AIN0
} ADS1247_VBIASAINx_t;     // 偏置电压

// MUX1 配置
typedef enum
{
    ADS1247_NORMAL,       // 正常模式
    ADS1247_OFFSET_CALIB, // 偏移校准
    ADS1247_GAIN_CALIB,   // 增益校准
    ADS1247_TEMP_MEASURE, // 温度测量
} ADS1247_RunMode_t;      // 运行模式

typedef enum
{
    ADS1247_REFP0_REFN0,          // 选择REFP0 和 REFN0的输入参考 (default)
    ADS1247_VREF_NOP,             // 空指令
    ADS1247_INTERNAL,             // 选择内部参考电压
    ADS1247_INTERNAL_REFP0_REFN0, // 选择内部参考并内部连接到 REFP0 和 REFN0 输入引脚
} ADS1247_VREF_t;                 // ref配置

typedef enum
{
    ADS1247_ALWAYS_OFF,  // 内部参考总是关闭
    ADS1247_ALWAYS_ON,   // 内部参考总是打开
    ADS1247_REF_DISABLE, // 内部参考在 START 低电平或 SLEEP 命令时关闭
} ADS1247_Internal_t;    // 内部参考设置

typedef enum
{
    ADS1247_EXCITA_CURRENT_OFF,    // 关闭激励电流
    ADS1247_EXCITA_CURRENT_50uA,   // 激励电流50uA
    ADS1247_EXCITA_CURRENT_100uA,  // 激励电流100uA
    ADS1247_EXCITA_CURRENT_250uA,  // 激励电流250uA
    ADS1247_EXCITA_CURRENT_500uA,  // 激励电流500uA
    ADS1247_EXCITA_CURRENT_750uA,  // 激励电流750uA
    ADS1247_EXCITA_CURRENT_1000uA, // 激励电流1000uA
    ADS1247_EXCITA_CURRENT_1500uA, // 激励电流1500uA
} ADS1247_EXCITACurrent_t;         // 激励电流大小

typedef enum
{
    ADS1247_DRDY_ONLY, // DOUT引脚只做数据输出引脚，不做DRDY信号 (default)
    ADS1247_DRDY_DOUT, // DOUT引脚既做数据输出，又做DRDY信号
} ADS1247_DRDYMode_t;

// IDAC1 配置

typedef enum
{
    ADS1247_IDAC_OUTPUT_AIN0,
    ADS1247_IDAC_OUTPUT_AIN1,
    ADS1247_IDAC_OUTPUT_AIN2,
    ADS1247_IDAC_OUTPUT_AIN3,
} ADS1247_IDACOutputDIR_t; // 激励电流输出方向设置

typedef enum
{
    ADS1247_GPIO_OUTPUT,
    ADS1247_GPIO_INPUT,
} ADS1247_GPIODIR_t; // GPIO方向

// GPIODAT_REG 配置
typedef enum
{
    ADS1247_GPIO_LOW,
    ADS1247_GPIO_HIGH,
} ADS1247_GPIOData_t;

typedef enum
{
    ADS1247_GPIO0,
    ADS1247_GPIO1,
    ADS1247_GPIO2,
    ADS1247_GPIO3,
} ADS1247_Pin_t;

typedef struct
{
    ADS1247_FlagSwitch_t DRDY; // DRDY信号 只读
#if ADS1120_USEDMA
    ADS1247_FlagSwitch_t DMA; // 是否启用DMA
#endif
} ADS1247_Flag_t;

typedef enum
{
    SAMPLE_RATE_5SPS,    // /< 每秒 5 次采样
    SAMPLE_RATE_10SPS,   // /< 每秒 10 次采样
    SAMPLE_RATE_20SPS,   // /< 每秒 20 次采样
    SAMPLE_RATE_40SPS,   // /< 每秒 40 次采样
    SAMPLE_RATE_80SPS,   // /< 每秒 80 次采样
    SAMPLE_RATE_160SPS,  // /< 每秒 160 次采样
    SAMPLE_RATE_320SPS,  // /< 每秒 320 次采样
    SAMPLE_RATE_640SPS,  // /< 每秒 640 次采样
    SAMPLE_RATE_1000SPS, // /< 每秒 1000 次采样
    SAMPLE_RATE_2000SPS,
} ADS1247_SampleRate_t; // 采样速率枚举

typedef enum
{
    PGA_GAIN_1X,   // PGA 1倍增益
    PGA_GAIN_2X,   // PGA 2倍增益
    PGA_GAIN_4X,   // PGA 4倍增益
    PGA_GAIN_8X,   // PGA 8倍增益
    PGA_GAIN_16X,  // PGA 16倍增益
    PGA_GAIN_32X,  // PGA 32倍增益
    PGA_GAIN_64X,  // PGA 64倍增益
    PGA_GAIN_128X, // PGA 128倍增ADS1247_PGA_Gain_t;         // PGA增益枚举
} ADS1247_PGA_Gain_t;

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
} ADS1247_GPIO_t;

typedef struct
{

    SPI_HandleTypeDef *spi; // 硬件spi
    ADS1247_GPIO_t CS;      // cs
    ADS1247_GPIO_t ReSet;   // 复位引脚
    ADS1247_GPIO_t Start;   // START引脚
    uint16_t Timeout;       // 超时时间
} ADS1247_Config_t;

typedef union
{
    struct
    {
        ADS1247_Flag_t flag;   // 标志位
        ADS1247_Config_t conf; // 配置
    };
} ADS1247_Class_t;

typedef ADS1247_Class_t *ADS1247_Handle_t;

extern volatile ADS1247_Class_t ads1247;

extern ADS1247_Staus_t ADS1247_getADC(ADS1247_Handle_t *handle, uint32_t *buffer);

extern ADS1247_Staus_t ADS1247_Callback(ADS1247_Handle_t *handle);

extern ADS1247_Staus_t ADS1247_CreateDevice(ADS1247_Handle_t *handle, ADS1247_Config_t *conf);

extern ADS1247_Staus_t ADS1247_SetDataRateAndPGA(ADS1247_Handle_t *handle, ADS1247_SampleRate_t dataRate, ADS1247_PGA_Gain_t pgaGain);

extern ADS1247_Staus_t ADS1247_Readbyte(ADS1247_Handle_t *handle, uint8_t *data, size_t size);

extern ADS1247_Staus_t ADS1247_Reset(ADS1247_Handle_t *handle);

extern ADS1247_Staus_t ADS1247_Delete(ADS1247_Handle_t *handle); // 删除ADS1247

extern ADS1247_Staus_t ADS1247_ConfigureVBIAS(ADS1247_Handle_t *handle, ADS1247_VBIASAINx_t VBIAS_AIN0, ADS1247_VBIASAINx_t VBIAS_AIN1, ADS1247_VBIASAINx_t VBIAS_AIN2, ADS1247_VBIASAINx_t VBIAS_AIN3);

extern ADS1247_Staus_t ADS1247_GPIO_Config(ADS1247_Handle_t *handle, ADS1247_Pin_t pin, ADS1247_GPIODIR_t DIR);

extern ADS1247_Staus_t ADS1247_GPIO_WritePin(ADS1247_Handle_t *handle, ADS1247_Pin_t pin, ADS1247_GPIOData_t data);

extern ADS1247_Staus_t ADS1247_SetReference(ADS1247_Handle_t *handle, ADS1247_VREF_t vref, ADS1247_Internal_t internal);

extern ADS1247_Staus_t ADS1247_SetMuxAndBSC(ADS1247_Handle_t *handle, ADS1247_Mux_t Mux_SP, ADS1247_Mux_t Mux_SN, ADS1247_BSC_t BSC);

extern ADS1247_Staus_t ADS1247_SetIDACWithDRDYMode(ADS1247_Handle_t *handle, ADS1247_EXCITACurrent_t IDAC, ADS1247_DRDYMode_t DRDY_Mode);

extern ADS1247_Staus_t ADS1247_SetIDACOutputPin(ADS1247_Handle_t *handle, ADS1247_IDACOutputDIR_t Output1, ADS1247_IDACOutputDIR_t Output2);
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // !ADS1247_H
