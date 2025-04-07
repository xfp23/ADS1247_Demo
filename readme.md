
# ADS1247 API 文档

该文档详细介绍了ADS1247模块的API接口，包括初始化、复位、配置、数据读取等功能。

---

## 1. ADS1247_CreateDevice

初始化ADS1247设备并创建设备句柄。

### 函数原型

```c
ADS1247_Status_t ADS1247_CreateDevice(ADS1247_Handle_t *handle, ADS1247_Config_t *conf);
```

### 参数

- `handle`: 输出参数，返回设备句柄。
- `conf`: 配置结构体。

### 返回值

- `ADS1247_OK`: 操作成功。
- `ADS1247_ERROR`: 发生错误。

---

## 2. ADS1247_Delete

删除ADS1247设备，释放相关资源。

### 函数原型

```c
ADS1247_Status_t ADS1247_Delete(ADS1247_Handle_t *handle);
```

### 参数

- `handle`: ADS1247设备句柄地址。

### 返回值

- `ADS1247_OK`: 操作成功。
- `ADS1247_ERROR`: 发生错误。

---

## 3. ADS1247_Reset

复位ADS1247设备。

### 函数原型

```c
ADS1247_Status_t ADS1247_Reset(ADS1247_Handle_t *handle);
```

### 参数

- `handle`: ADS1247设备句柄地址。

### 返回值

- `ADS1247_OK`: 操作成功。
- `ADS1247_ERROR`: 发生错误。

---

## 4. ADS1247_Writebyte

向ADS1247设备写入数据。

### 函数原型

```c
ADS1247_Status_t ADS1247_Writebyte(ADS1247_Handle_t *handle, uint8_t *data, size_t size);
```

### 参数

- `handle`: ADS1247设备句柄地址。
- `data`: 要写入的数据。
- `size`: 数据的大小（单位：字节）。

### 返回值

- `ADS1247_OK`: 操作成功。
- `ADS1247_SPIERROR`: SPI传输错误。
- `ADS1247_ERROR`: 发生错误。

---

## 5. ADS1247_Readbyte

从ADS1247设备读取数据。

### 函数原型

```c
ADS1247_Status_t ADS1247_Readbyte(ADS1247_Handle_t *handle, uint8_t *data, size_t size);
```

### 参数

- `handle`: ADS1247设备句柄地址。
- `data`: 存储读取数据的缓冲区。
- `size`: 要读取的数据大小（单位：字节）。

### 返回值

- `ADS1247_OK`: 操作成功。
- `ADS1247_SPIERROR`: SPI读取错误。
- `ADS1247_ERROR`: 发生错误。

---

## 6. ADS1247_WriteCommand

向ADS1247设备发送命令。

### 函数原型

```c
ADS1247_Status_t ADS1247_WriteCommand(ADS1247_Handle_t *handle, ADS1247_Command_t cmd);
```

### 参数

- `handle`: ADS1247设备句柄地址。
- `cmd`: 要发送的命令。

### 返回值

- `ADS1247_OK`: 操作成功。
- `ADS1247_ERROR`: 发生错误。

---

## 7. ADS1247_WriteReg

向指定寄存器写入数据。

### 函数原型

```c
ADS1247_Status_t ADS1247_WriteReg(ADS1247_Handle_t *handle, uint8_t reg, uint8_t conf);
```

### 参数

- `handle`: ADS1247设备句柄地址。
- `reg`: 目标寄存器。
- `conf`: 配置字节。

### 返回值

- `ADS1247_OK`: 操作成功。
- `ADS1247_ERROR`: 发生错误。

---

## 8. ADS1247_ReadReg

读取指定寄存器的数据。

### 函数原型

```c
ADS1247_Status_t ADS1247_ReadReg(ADS1247_Handle_t *handle, uint8_t reg, uint8_t *buffer);
```

### 参数

- `handle`: ADS1247设备句柄地址。
- `reg`: 要读取的寄存器。
- `buffer`: 存储读取数据的缓冲区。

### 返回值

- `ADS1247_OK`: 操作成功。
- `ADS1247_ERROR`: 发生错误。

---

## 9. ADS1247_getADC

获取ADS1247的ADC转换结果。

### 函数原型

```c
ADS1247_Status_t ADS1247_getADC(ADS1247_Handle_t *handle, uint32_t *buffer);
```

### 参数

- `handle`: ADS1247设备句柄地址。
- `buffer`: 存储ADC结果的缓冲区。

### 返回值

- `ADS1247_OK`: 操作成功。
- `ADS1247_DRDYERROR`: 数据就绪错误。
- `ADS1247_ERROR`: 发生错误。

---

## 10. ADS1247_SetDataRateAndPGA

设置数据采样率和PGA增益。

### 函数原型

```c
ADS1247_Status_t ADS1247_SetDataRateAndPGA(ADS1247_Handle_t *handle, ADS1247_SampleRate_t dataRate, ADS1247_PGA_Gain_t pgaGain);
```

### 参数

- `handle`: ADS1247设备句柄地址。
- `dataRate`: 数据采样率。
- `pgaGain`: PGA增益。

### 返回值

- `ADS1247_OK`: 操作成功。
- `ADS1247_ERROR`: 发生错误。

---

## 11. ADS1247_ConfigureVBIAS

配置偏置电压。

### 函数原型

```c
ADS1247_Status_t ADS1247_ConfigureVBIAS(ADS1247_Handle_t *handle,
                                        ADS1247_VBIASAINx_t VBIAS_AIN0,
                                        ADS1247_VBIASAINx_t VBIAS_AIN1,
                                        ADS1247_VBIASAINx_t VBIAS_AIN2,
                                        ADS1247_VBIASAINx_t VBIAS_AIN3);
```

### 参数

- `handle`: ADS1247设备句柄地址。
- `VBIAS_AIN0`: AIN0的偏置电压配置。
- `VBIAS_AIN1`: AIN1的偏置电压配置。
- `VBIAS_AIN2`: AIN2的偏置电压配置。
- `VBIAS_AIN3`: AIN3的偏置电压配置。

### 返回值

- `ADS1247_OK`: 操作成功。
- `ADS1247_ERROR`: 发生错误。

---

## 12. ADS1247_SetIDACOutputPin

配置IDAC输出引脚。

### 函数原型

```c
ADS1247_Status_t ADS1247_SetIDACOutputPin(ADS1247_Handle_t *handle, ADS1247_IDACOutputDIR_t Output1, ADS1247_IDACOutputDIR_t Output2);
```

### 参数

- `handle`: ADS1247设备句柄地址。
- `Output1`: 第一个IDAC输出引脚。
- `Output2`: 第二个IDAC输出引脚。

### 返回值

- `ADS1247_OK`: 操作成功。
- `ADS1247_ERROR`: 发生错误。

---

## 13. ADS1247_SetIDACWithDRDYMode

设置IDAC与DRDY模式。

### 函数原型

```c
ADS1247_Status_t ADS1247_SetIDACWithDRDYMode(ADS1247_Handle_t *handle, ADS1247_EXCITACurrent_t IDAC, ADS1247_DRDYMode_t DRDY_Mode);
```

### 参数

- `handle`: ADS1247设备句柄地址。
- `IDAC`: IDAC电流配置。
- `DRDY_Mode`: DRDY模式。

### 返回值

- `ADS1247_OK`: 操作成功。
- `ADS1247_ERROR`: 发生错误。

---

## 14. ADS1247_SetMuxAndBSC

设置多路复用器和基准选择。

### 函数原型

```c
ADS1247_Status_t ADS1247_SetMuxAndBSC(ADS1247_Handle_t *handle, ADS1247_Mux_t Mux_SP, ADS1247_Mux_t Mux_SN, ADS1247_BSC_t BSC);
```

### 参数

- `handle`: ADS1247设备句柄地址。
- `Mux_SP`: 选择的正输入端。
- `Mux_SN`: 选择的负输入端。
- `BSC`: 基准选择。

### 返回值

- `ADS1247_OK`: 操作成功。
- `ADS1247_ERROR`: 发生错误。

---

## 15. ADS1247_Callback

该回调函数用于处理设备的中断。必须将此 API 放入 DRDY 中断引脚的触发函数中，以确保设备能够正常工作。中断应在下降沿触发时激活。

### 函数原型

```c
ADS1247_Status_t ADS1247_Callback(ADS1247_Handle_t *handle);
```

### 参数

- `handle`: ADS1247设备句柄地址。

### 返回值

- `ADS1247_OK`: 操作成功。
- `ADS1247_ERROR`: 发生错误。

---

## 16. ADS1247_GPIO_WritePin

控制GPIO引脚输出状态。

### 函数原型

```c
ADS1247_Status_t ADS1247_GPIO_WritePin(ADS1247_Handle_t *handle, ADS1247_Pin_t pin, ADS1247_GPIOData_t data);
```

### 参数

- `handle`: ADS1247设备句柄地址。
- `pin`: GPIO引脚。
- `value`: 输出值。

### 返回值

- `ADS1247_OK`: 操作成功。
- `ADS1247_ERROR`: 发生错误。

---

# ADS1247 枚举类型文档

## 1. ADS1247_Staus_t

### 枚举类型：设备状态

该枚举类型定义了 ADS1247 设备的各种状态。

值：

- **ADS1247_OK**  
  设备正常运行。

- **ADS1247_SPIERROR**  
  SPI 通信错误。

- **ADS1247_ERROR**  
  一般错误。

- **ADS1247_DRDYERROR**  
  数据准备好（DRDY）错误。

---

## 2. ADS1247_FlagSwitch_t

### 枚举类型：标志开关控制

该枚举类型定义了标志的开关状态。

值：

- **ADS1247_Flag_OFF (0x00)**  
  标志关闭。

- **ADS1247_Flag_ON (0x01)**  
  标志开启。

---

## 3. ADS1247_Mux_t

### 枚举类型：多路复用配置

该枚举类型定义了 ADS1247 的多路复用配置。

值：

- **ADS1247_MUX_AIN0**  
  配置为 AIN0。

- **ADS1247_MUX_AIN1**  
  配置为 AIN1。

- **ADS1247_MUX_AIN2**  
  配置为 AIN2。

- **ADS1247_MUX_AIN3**  
  配置为 AIN3。

---

## 4. ADS1247_BSC_t

### 枚举类型：烧毁电流源选择

该枚举类型定义了 ADS1247 的烧毁电流源选项。

值：

- **ADS1247_BSC_OFF**  
  关闭烧毁电流源。

- **ADS1247_BSC_0_5UA**  
  设置烧毁电流为 0.5uA。

- **ADS1247_BSC_2UA**  
  设置烧毁电流为 2uA。

- **ADS1247_BSC_10UA**  
  设置烧毁电流为 10uA。

---

## 5. ADS1247_VBIASAINx_t

### 枚举类型：偏置电压配置

该枚举类型定义了 ADS1247 偏置电压配置。

值：

- **ADS1247_VBIAS_DISABLE**  
  禁用 AIN0 的偏置电压。

- **ADS1247_VBIAS_ENABLE**  
  启用 AIN0 的偏置电压（中间电源 (AVDD + AVSSE) / 2）。

---

## 6. ADS1247_RunMode_t

### 枚举类型：运行模式

该枚举类型定义了 ADS1247 的运行模式。

值：

- **ADS1247_NORMAL**  
  正常模式。

- **ADS1247_OFFSET_CALIB**  
  偏移校准模式。

- **ADS1247_GAIN_CALIB**  
  增益校准模式。

- **ADS1247_TEMP_MEASURE**  
  温度测量模式。

---

## 7. ADS1247_VREF_t

### 枚举类型：参考电压配置

该枚举类型定义了 ADS1247 的参考电压配置。

值：

- **ADS1247_REFP0_REFN0**  
  选择 REFP0 和 REFN0 的输入参考（默认设置）。

- **ADS1247_VREF_NOP**  
  空指令。

- **ADS1247_INTERNAL**  
  选择内部参考电压。

- **ADS1247_INTERNAL_REFP0_REFN0**  
  选择内部参考并内部连接到 REFP0 和 REFN0 输入引脚。

---

## 8. ADS1247_Internal_t

### 枚举类型：内部参考设置

该枚举类型定义了 ADS1247 的内部参考设置。

值：

- **ADS1247_ALWAYS_OFF**  
  内部参考总是关闭。

- **ADS1247_ALWAYS_ON**  
  内部参考总是开启。

- **ADS1247_REF_DISABLE**  
  内部参考在 START 低电平或 SLEEP 命令时关闭。

---

## 9. ADS1247_EXCITACurrent_t

### 枚举类型：激励电流大小

该枚举类型定义了 ADS1247 的激励电流大小。

值：

- **ADS1247_EXCITA_CURRENT_OFF**  
  关闭激励电流。

- **ADS1247_EXCITA_CURRENT_50uA**  
  激励电流为 50uA。

- **ADS1247_EXCITA_CURRENT_100uA**  
  激励电流为 100uA。

- **ADS1247_EXCITA_CURRENT_250uA**  
  激励电流为 250uA。

- **ADS1247_EXCITA_CURRENT_500uA**  
  激励电流为 500uA。

- **ADS1247_EXCITA_CURRENT_750uA**  
  激励电流为 750uA。

- **ADS1247_EXCITA_CURRENT_1000uA**  
  激励电流为 1000uA。

- **ADS1247_EXCITA_CURRENT_1500uA**  
  激励电流为 1500uA。

---

## 10. ADS1247_DRDYMode_t

### 枚举类型：DRDY 模式

该枚举类型定义了 DOUT 引脚的功能配置。

值：

- **ADS1247_DRDY_ONLY**  
  DOUT 引脚只做数据输出引脚，不做 DRDY 信号（默认设置）。

- **ADS1247_DRDY_DOUT**  
  DOUT 引脚既做数据输出，也做 DRDY 信号。

---

## 11. ADS1247_IDACOutputDIR_t

### 枚举类型：激励电流输出方向设置

该枚举类型定义了 ADS1247 的激励电流输出方向设置。

值：

- **ADS1247_IDAC_OUTPUT_AIN0**  
  输出到 AIN0。

- **ADS1247_IDAC_OUTPUT_AIN1**  
  输出到 AIN1。

- **ADS1247_IDAC_OUTPUT_AIN2**  
  输出到 AIN2。

- **ADS1247_IDAC_OUTPUT_AIN3**  
  输出到 AIN3。

---

## 12. ADS1247_GPIODIR_t

### 枚举类型：GPIO 方向设置

该枚举类型定义了 ADS1247 的 GPIO 方向设置。

值：

- **ADS1247_GPIO_OUTPUT**  
  设置为输出模式。

- **ADS1247_GPIO_INPUT**  
  设置为输入模式。

---

## 13. ADS1247_GPIOData_t

### 枚举类型：GPIO 数据设置

该枚举类型定义了 ADS1247 的 GPIO 数据设置。

值：

- **ADS1247_GPIO_LOW**  
  设置 GPIO 为低电平。

- **ADS1247_GPIO_HIGH**  
  设置 GPIO 为高电平。

---

## 14. ADS1247_Pin_t

### 枚举类型：GPIO 引脚选择

该枚举类型定义了 ADS1247 的 GPIO 引脚。

值：

- **ADS1247_GPIO0**  
  GPIO 引脚 0。

- **ADS1247_GPIO1**  
  GPIO 引脚 1。

- **ADS1247_GPIO2**  
  GPIO 引脚 2。

- **ADS1247_GPIO3**  
  GPIO 引脚 3。

- **ADS1247_GPIOALL**  
  仅在复位时使用。

---

## 15. ADS1247_SampleRate_t

### 枚举类型：采样速率设置

该枚举类型定义了 ADS1247 的采样速率。

值：

- **SAMPLE_RATE_5SPS**  
  每秒 5 次采样。

- **SAMPLE_RATE_10SPS**  
  每秒 10 次采样。

- **SAMPLE_RATE_20SPS**  
  每秒 20 次采样。

- **SAMPLE_RATE_40SPS**  
  每秒 40 次采样。

- **SAMPLE_RATE_80SPS**  
  每秒 80 次采样。

- **SAMPLE_RATE_160SPS**  
  每秒 160 次采样。

- **SAMPLE_RATE_320SPS**  
  每秒 320 次采样。

- **SAMPLE_RATE_640SPS**  
  每秒 640 次采样。

- **SAMPLE_RATE_1000SPS**  
  每秒 1000 次采样。

- **SAMPLE_RATE_2000SPS**  
  每秒 2000 次采样。

---

## 16. ADS1247_PGA_Gain_t

### 枚举类型：PGA 增益设置

该枚举类型定义了 ADS1247 的 PGA 增益。

值：

- **PGA_GAIN_1X**  
  PGA 增益 1 倍。

- **PGA_GAIN_2X**  
  PGA 增益 2 倍。

- **PGA_GAIN_4X**  
  PGA 增益 4 倍。

- **PGA_GAIN_8X**  
  PGA 增益 8 倍。

- **PGA_GAIN_16X**  
  PGA 增益 16 倍。

- **PGA_GAIN_32X**  
  PGA 增益 32 倍。

- **PGA_GAIN_64X**  
  PGA 增益 64 倍。

- **PGA_GAIN_128X**  
  PGA 增益 128 倍。