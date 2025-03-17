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

// ADS1247 spi传输超时时间
#define ADS1247_TIMEOUT 100

// 硬件端口

// 硬件spi 时钟低极性,第二边沿采集
#define ADS1247_SPI_Handle &hspi1

// CS引脚
#define ADS1247_CS_Port GPIOA
#define ADS1247_CS_Pin GPIO_PIN_4

// start引脚
#define ADS1247_START_Port ADS_START_GPIO_Port
#define ADS1247_START_Pin ADS_START_Pin

// Reset引脚
#define ADS1247_RESET_Port ADS_RESET_GPIO_Port
#define ADS1247_RESET_Pin ADS_RESET_Pin

// 拉高CS引脚
#define ADS1247_CS_SET HAL_GPIO_WritePin(ADS1247_CS_Port, ADS1247_CS_Pin, GPIO_PIN_SET)
// 拉低CS引脚
#define ADS1247_CS_CLR HAL_GPIO_WritePin(ADS1247_CS_Port, ADS1247_CS_Pin, GPIO_PIN_RESET)

// 拉高start引脚
#define ADS1247_START_SET HAL_GPIO_WritePin(ADS1247_START_Port, ADS1247_START_Pin, GPIO_PIN_SET)
// 拉低start引脚
#define ADS1247_START_CLR HAL_GPIO_WritePin(ADS1247_START_Port, ADS1247_START_Pin, GPIO_PIN_RESET)

// 拉高Reset引脚
#define ADS1247_RESET_SET HAL_GPIO_WritePin(ADS1247_RESET_Port, ADS1247_RESET_Pin, GPIO_PIN_SET)
// 拉低start引脚
#define ADS1247_RESET_CLR HAL_GPIO_WritePin(ADS1247_RESET_Port, ADS1247_RESET_Pin, GPIO_PIN_RESET)

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
    IDAC0_REG = 0x0A,   // 激励电流
    IDAC1_REG = 0x0B,   // 激励电流
    GPIOCFG_REG = 0x0C, // GPIO配置
    GPIODIR_REG = 0x0C, // GPIO方向
    GPIODAT_REG = 0x0E,

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

// mux0配置

// ADC反向输入选择
#define MUX_SN_AIN0 0x00 // AIN0负输入
#define MUX_SN_AIN1 0x01 // AIN1负输入
#define MUX_SN_AIN2 0x02 // AIN2负输入
#define MUX_SN_AIN3 0x03 // AIN3负输入

// ADC正向输入选择
#define MUX_SP_AIN0 (0x00 << 3) // AIN1正输入
#define MUX_SP_AIN1 (0x01 << 3) // AIN2正输入
#define MUX_SP_AIN2 (0x02 << 3) // AIN3正输入
#define MUX_SP_AIN3 (0x04 << 3) // AIN4正输入

// 电流源配置
#define BSC_OFF (0x00 << 6)   // 关闭电流源
#define BSC_0_5UA (0x01 << 6) // 0.5uA
#define BSC_2UA (0x02 << 6)   // 2uA
#define BSC_10UA (0x03 << 6)  // 10uA

// VBIAS 偏置电压配置

#define VBIAS_AIN0_DISABLE 0x00 // 禁用 AIN0 的偏置电压
#define VBIAS_AIN0_ENABLE 0x01  // 将中间电源(AVDD + AVSSE) / 2 的偏置电压施加到AIN0

#define VBIAS_AIN1_DISABLE (0x00 << 1) // 禁用 AIN1 的偏置电压
#define VBIAS_AIN1_ENABLE (0x01 << 1)  // 将中间电源(AVDD + AVSSE) / 2 的偏置电压施加到AIN1

#define VBIAS_AIN2_DISABLE (0x00 << 2) // 禁用 AIN1 的偏置电压
#define VBIAS_AIN2_ENABLE (0x01 << 2)  // 将中间电源(AVDD + AVSSE) / 2 的偏置电压施加到AIN2

#define VBIAS_AIN3_DISABLE (0x00 << 3) // 禁用 AIN1 的偏置电压
#define VBIAS_AIN3_ENABLE (0x01 << 3)  // 将中间电源(AVDD + AVSSE) / 2 的偏置电压施加到AIN3

// MUX1 配置
#define NORMAL_MODEL 0x00 // 正常模式
#define OFFSET_CALIB 0x01 // 偏移校准
#define GAIN_CALIB 0x02   // 增益校准
#define TEMP_MEASURE 0x03 // 温度测量

#define REFSELT_REFP0_REFN0 (0x00 << 3)         // 选择REFP0 和 REFN0的输入参考 (default)
#define REFSELT_INTERNAL (0x02 << 3)            // 选择内部参考电压
#define REFSELT_INTERNAL_REFPO_REFN0 0x03 << 3 )// 选择内部参考并内部连接到 REFP0 和 REFN0 输入引脚

#define INTERNAL_ALWAYS_OFF (0x00 << 5)  // 内部参考总是关闭
#define INTERNAL_ALWAYS_ON (0x01 << 5)   // 内部参考总是打开
#define INTERNAL_REF_DISABLE (0x02 << 5) // 内部参考在 START 低电平或 SLEEP 命令时关闭

// SYS0配置

#define DR_5SPS 0x00    // 每秒五次 (default)
#define DR_10SPS 0x01   // 每秒十次
#define DR_20SPS 0x02   // 每秒20次
#define DR_40SPS 0x03   // 每秒2000次
#define DR_80SPS 0x04   // 每秒2000次
#define DR_160SPS 0x05  // 每秒2000次
#define DR_320SPS 0x06  // 每秒2000次
#define DR_640SPS 0x07  // 每秒2000次
#define DR_1000SPS 0x08 // 每秒2000次
#define DR_2000SPS 0x09 // 每秒2000次

#define PGA_1X (0x00 << 4) // (default)
#define PGA_2X (0x01 << 4)
#define PGA_4X (0x02 << 4)
#define PGA_8X (0x03 << 4)
#define PGA_16X (0x04 << 4)
#define PGA_32X (0x05 << 4)
#define PGA_64X (0x06 << 4)
#define PGA_128X (0x07 << 4)

// IDAC0 配置
#define EXCITA_CURRENT_OFF 0x00    // 关闭激励电流
#define EXCITA_CURRENT_50uA 0x01   // 激励电流50uA
#define EXCITA_CURRENT_100uA 0x02  // 激励电流100uA
#define EXCITA_CURRENT_250uA 0x03  // 激励电流250uA
#define EXCITA_CURRENT_500uA 0x04  // 激励电流500uA
#define EXCITA_CURRENT_750uA 0x05  // 激励电流750uA
#define EXCITA_CURRENT_1000uA 0x06 // 激励电流1000uA
#define EXCITA_CURRENT_1500uA 0x07 // 激励电流1500uA

#define DRDY_MODE_DOUT_ONLY (0x00 << 3) // DOUT引脚只做数据输出引脚，不做DRDY信号 (default)
#define DRDY_MODE_DOUT_DRDY (0x01 << 3) // DOUT引脚既做数据输出，又做DRDY信号

// IDAC1 配置

// 选择第二个激励电流源
#define IDAC1_OUTPUT2_AIN0 0x00 // 选择AIN0引脚输出激励电流
#define IDAC1_OUTPUT2_AIN1 0x01 // 选择AIN1引脚输出激励电流
#define IDAC1_OUTPUT2_AIN2 0x02 // 选择AIN2引脚输出激励电流
#define IDAC1_OUTPUT2_AIN3 0x03 // 选择AIN3引脚输出激励电流

// 选择第一个激励电流源
#define IDAC1_OUTPUT1_AIN0 (0x00 << 4) // 选择AIN0引脚输出激励电流
#define IDAC1_OUTPUT1_AIN1 (0x01 << 4) // 选择AIN1引脚输出激励电流
#define IDAC1_OUTPUT1_AIN2 (0x02 << 4) // 选择AIN2引脚输出激励电流
#define IDAC1_OUTPUT1_AIN3 (0x03 << 4) // 选择AIN3引脚输出激励电流

// GPIOCFG 配置
#define GPIO0_DISABLE 0X00       // 不启用GPIO0
#define GPIO0_APPLIED_REFP0 0x01 // GPIO0作用于REFP0

#define GPIO1_DISABLE (0x00 << 1)       // 不启用GPIO1
#define GPIO1_APPLIED_REFN0 (0x01 << 1) // GPIO1作用于REFN0

#define GPIO2_DISABLE (0x00 << 2)      // 不启用gpio2
#define GPIO2_APPLIED_AIN2 (0x01 << 2) // gpio2 作用于AIN2

#define GPIO3_DISABLE (0x00 << 3)      // gpio3 不启用
#define GPIO3_APPLIED_AIN3 (0X01 << 3) // GPIO3 作用于AIN3

// GPIODIR 配置
#define GPIO0_DIR_OUTPUT 0x00 // GPIO0 设为输出
#define GPIO0_DIR_INPUT 0x01  // GPIO0 设为输入

#define GPIO1_DIR_OUTPUT (0x00 << 1) // GPIO1 设为输出
#define GPIO1_DIR_INPUT (0x01 << 1)  // GPIO1 设为输入

#define GPIO2_DIR_OUTPUT (0x00 << 2) // GPIO2 设为输出
#define GPIO2_DIR_INPUT (0x01 << 2)  // GPIO2 设为输入

#define GPIO3_DIR_OUTPUT (0x00 << 3) // GPIO3 设为输出
#define GPIO3_DIR_INPUT (0x01 << 3)  // GPIO3 设为输入

// GPIODAT_REG 配置
#define GPIO0_DAT_LOW 0x00  // 向 GPIO0 写入低电平
#define GPIO0_DAT_HIGH 0x01 // 向 GPIO0 写入高电平

#define GPIO1_DAT_LOW (0x00 << 1)  // 向 GPIO1 写入低电平
#define GPIO1_DAT_HIGH (0x01 << 1) // 向 GPIO1 写入高电平

#define GPIO2_DAT_LOW (0x00 << 2)  // 向 GPIO2 写入低电平
#define GPIO2_DAT_HIGH (0x01 << 2) // 向 GPIO2 写入高电平

#define GPIO3_DAT_LOW (0x00 << 3)  // 向 GPIO3 写入低电平
#define GPIO3_DAT_HIGH (0x01 << 3) // 向 GPIO3 写入高电平

typedef struct
{
    uint16_t DRDY; // DRDY信号

} ADS1247_Flag_t;

typedef enum
{
    SAMPLE_RATE_5SPS = DR_5SPS,       // /< 每秒 5 次采样
    SAMPLE_RATE_10SPS = DR_10SPS,     // /< 每秒 10 次采样
    SAMPLE_RATE_20SPS = DR_20SPS,     // /< 每秒 20 次采样
    SAMPLE_RATE_40SPS = DR_40SPS,     // /< 每秒 40 次采样
    SAMPLE_RATE_80SPS = DR_80SPS,     // /< 每秒 80 次采样
    SAMPLE_RATE_160SPS = DR_160SPS,   // /< 每秒 160 次采样
    SAMPLE_RATE_320SPS = DR_320SPS,   // /< 每秒 320 次采样
    SAMPLE_RATE_640SPS = DR_640SPS,   // /< 每秒 640 次采样
    SAMPLE_RATE_1000SPS = DR_1000SPS, // /< 每秒 1000 次采样
    SAMPLE_RATE_2000SPS = DR_2000SPS, // /< 每秒 2000 次采样
} ADS1247_SampleRate_t;               // 采样速率枚举

typedef enum
{
    PGA_GAIN_1X = PGA_1X,     // PGA 1倍增益
    PGA_GAIN_2X = PGA_2X,     // PGA 2倍增益
    PGA_GAIN_4X = PGA_4X,     // PGA 4倍增益
    PGA_GAIN_8X = PGA_8X,     // PGA 8倍增益
    PGA_GAIN_16X = PGA_16X,   // PGA 16倍增益
    PGA_GAIN_32X = PGA_32X,   // PGA 32倍增益
    PGA_GAIN_64X = PGA_64X,   // PGA 64倍增益
    PGA_GAIN_128X = PGA_128X, // PGA 128倍增益
} ADS1247_PGA_Gain_t;         // PGA增益枚举

typedef struct
{
    ADS1247_SampleRate_t DR; // 数据采样速率
    ADS1247_PGA_Gain_t PGA;  // PGA
} ADS1247_Config_t;

typedef union
{
    struct
    {
        ADS1247_Flag_t flag;   // 标志位
        ADS1247_Config_t conf; // 配置
    };
} ADS1247_Class_t;

extern volatile ADS1247_Class_t ads1247;

extern uint32_t ADS1247_getADC(void);

extern void ADS1247_Callback(void);

extern void ADS1247_Init(void);

extern uint8_t ADS1247_SetDataRateAndPGA(ADS1247_SampleRate_t dataRate, ADS1247_PGA_Gain_t pgaGain);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // !ADS1247_H
