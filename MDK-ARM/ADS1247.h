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
    VBIAS = 0x01, // 烧毁电流源
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

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // !ADS1247_H
