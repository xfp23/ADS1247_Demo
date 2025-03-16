#ifndef ADS1247_H
#define ADS1247_H

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

// CS引脚
#define ADS1247_CS_Port GPIOA
#define ADS1247_CS_Pin GPIO_PIN_4

// 拉高CS引脚
#define ADS1247_CS_SET HAL_GPIO_WritePin(ADS1247_CS_Port, ADS1247_CS_Pin, GPIO_PIN_SET)
// 拉低CS引脚
#define ADS1247_CS_CLR HAL_GPIO_WritePin(ADS1247_CS_Port, ADS1247_CS_Pin, GPIO_PIN_RESET)

// start引脚
#define ADS1247_START_Port ADS_START_GPIO_Port
#define ADS1247_START_Pin ADS_START_Pin

// 拉高start引脚
#define ADS1247_START_SET HAL_GPIO_WritePin(ADS1247_START_Port, ADS1247_START_Pin, GPIO_PIN_SET)
// 拉低start引脚
#define ADS1247_START_CLR HAL_GPIO_WritePin(ADS1247_START_Port, ADS1247_START_Pin, GPIO_PIN_RESET)

// Reset引脚
#define ADS1247_RESET_Port ADS_RESET_GPIO_Port
#define ADS1247_RESET_Pin ADS_RESET_Pin

// 拉高Reset引脚
#define ADS1247_RESET_SET HAL_GPIO_WritePin(ADS1247_RESET_Port, ADS1247_RESET_Pin, GPIO_PIN_SET)
// 拉低start引脚
#define ADS1247_RESET_CLR HAL_GPIO_WritePin(ADS1247_RESET_Port, ADS1247_RESET_Pin, GPIO_PIN_RESET)

// 硬件spi 时钟低极性,第二边沿采集
#define ADS1247_SPI_Handle &hspi1

// 寄存器枚举
// Table 29. ADS1247 and ADS1248 Register Map
typedef enum
{
    MUX0 = 0x00,  // mux 复用器0
    VBIAS = 0x01, // 偏置电压
    MUX1 = 0x02,  // mux复用器1
    SYS0 = 0x03,  // 系统设置PGA与采样速率寄存器
    OFC0 = 0x04,  // 系统偏移校准
    OFC1 = 0x05,  // 系统偏移校准
    OFC2 = 0x06,  // 系统偏移校准
    FSC0 = 0x07,  // 系统增益校准
    FSC1 = 0x08,  // 系统增益校准
    FSC2 = 0x09,  // 系统增益校准
    IDAC0 = 0x0A, // 激励电流
    IDAC1 = 0x0B, // 激励电流
    GPIOCFG = 0x0C,
    GPIODIR = 0x0C,
    GPIODAT = 0x0E,

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
#define MUX_SP_AIN0 0x00 << 3 // AIN1正输入
#define MUX_SP_AIN1 0x01 << 3 // AIN2正输入
#define MUX_SP_AIN2 0x02 << 3 // AIN3正输入
#define MUX_SP_AIN3 0x04 << 3 // AIN4正输入

// 电流源配置
#define BSC_OFF 0x00 << 6   // 关闭电流源
#define BSC_0_5UA 0x01 << 6 // 0.5uA
#define BSC_2UA 0x02 << 6   // 2uA
#define BSC_10UA 0x03 << 6  // 10uA

// VBIAS 偏置电压配置

#define VBIAS_AIN0_DISABLE 0x00 // 禁用 AIN0 的偏置电压
#define VBIAS_AIN0_ENABLE 0x01  // 将中间电源(AVDD + AVSSE) / 2 的偏置电压施加到AIN0

#define VBIAS_AIN1_DISABLE 0x00 << 1 // 禁用 AIN1 的偏置电压
#define VBIAS_AIN1_ENABLE 0x01 << 1  // 将中间电源(AVDD + AVSSE) / 2 的偏置电压施加到AIN1

#define VBIAS_AIN2_DISABLE 0x00 << 2 // 禁用 AIN1 的偏置电压
#define VBIAS_AIN2_ENABLE 0x01 << 2  // 将中间电源(AVDD + AVSSE) / 2 的偏置电压施加到AIN2

#define VBIAS_AIN3_DISABLE 0x00 << 3 // 禁用 AIN1 的偏置电压
#define VBIAS_AIN3_ENABLE 0x01 << 3  // 将中间电源(AVDD + AVSSE) / 2 的偏置电压施加到AIN3

// MUX1 配置
#define NORMAL_MODEL 0x00 // 正常模式
#define OFFSET_CALIB 0x01 // 偏移校准
#define GAIN_CALIB 0x02   // 增益校准
#define TEMP_MEASURE 0x03 // 温度测量

#define REFSELT_REFP0_REFN0 0x00 << 3          // 选择REFP0 和 REFN0的输入参考 (default)
#define REFSELT_INTERNAL 0x02 << 3             // 选择内部参考电压
#define REFSELT_INTERNAL_REFPO_REFN0 0x03 << 3 // 选择内部参考并内部连接到 REFP0 和 REFN0 输入引脚


#define INTERNAL_ALWAYS_OFF 0x00 << 5 // 内部参考总是关闭
#define INTERNAL_ALWAYS_ON 0x01 << 5 // 内部参考总是打开
#define INTERNAL_REF_DISABLE 0x02 << 5  // 内部参考在 START 低电平或 SLEEP 命令时关闭


// SYS0配置

#define DR_5SPS  0x00 // 每秒五次 (default)
#define DR_10SPS 0x01 // 每秒十次
#define DR_20SPS 0x02 // 每秒20次
#define DR_40SPS 0x03 // 每秒2000次
#define DR_80SPS 0x04 // 每秒2000次
#define DR_160SPS 0x05 // 每秒2000次
#define DR_320SPS 0x06 // 每秒2000次
#define DR_640SPS 0x07 // 每秒2000次
#define DR_1000SPS 0x08 // 每秒2000次
#define DR_2000SPS 0x09 // 每秒2000次

#define PGA_1X 0x00 << 4 // (default)
#define PGA_2X 0x01 << 4 
#define PGA_4X 0x02 << 4
#define PGA_8X 0x03 << 4
#define PGA_16X 0x04 << 4
#define PGA_32X 0x05 << 4
#define PGA_64X 0x06 << 4
#define PGA_128X 0x07 << 4


// IDAC0 配置
#define EXCITA_CURRENT_OFF 0x00 // 关闭激励电流 
#define EXCITA_CURRENT_50uA 0x01 // 激励电流50uA
#define EXCITA_CURRENT_100uA 0x02 // 激励电流100uA
#define EXCITA_CURRENT_250uA 0x03 // 激励电流250uA
#define EXCITA_CURRENT_500uA 0x04 // 激励电流500uA
#define EXCITA_CURRENT_750uA 0x05 // 激励电流750uA
#define EXCITA_CURRENT_1000uA 0x06 // 激励电流1000uA
#define EXCITA_CURRENT_1500uA 0x07 // 激励电流1500uA
 
#define DRDY_MODE_DOUT_ONLY 0x00 // DOUT引脚只做数据输出引脚，不做DRDY信号 (default)
#define DRDY_MODE_DOUT_DRDY 0x01 // DOUT引脚既做数据输出，又做DRDY信号

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // !ADS1247_H
