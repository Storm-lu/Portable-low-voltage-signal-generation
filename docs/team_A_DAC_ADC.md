# Team A - 模拟信号链 (DAC + ADC)

## 你负责的文件

| 文件 | 作用 |
|------|------|
| `HW/DAC/DAC.c` | DAC 直流/波形输出，DMA2+TIM4 配置 |
| `HW/DAC/DAC.h` | 接口声明（已有文档注释，按此实现） |
| `HW/DAC/Wave.c` | 100点波形查找表（三角波/正弦波/方波） |
| `HW/DAC/Wave.h` | 波形数据接口声明 |
| `HW/ADC/ADC.c` | ADC 采集，DMA1 循环缓冲，统计计算 |
| `HW/ADC/ADC.h` | 接口声明（已有文档注释，按此实现） |

## 硬件资源

| 外设 | 用途 | 引脚 |
|------|------|------|
| DAC Channel 1 | 模拟电压输出 | PA4 |
| DMA2 Channel 3 | DAC 波形数据传输 (Memory -> Peripheral) | - |
| TIM4 | DAC 触发定时器 (TRGO Update) | - |
| ADC1 Channel 1 | 模拟电压采集 | PA1 |
| DMA1 Channel 1 | ADC 数据传输 (Peripheral -> Memory) | - |

**无冲突**：DMA2 和 DMA1 是不同的 DMA 控制器，TIM4 和其他人的定时器不冲突。

## 需要实现的函数

### DAC.c

| 函数 | 做什么 | 谁调用 |
|------|--------|--------|
| `InitDAC()` | 初始化 PA4、DAC1、TIM4。默认三角波，采样率 10kHz | Main.c -> InitHardware() |
| `StartDAC(u16 dacValue)` | 直流输出。关 DMA，直接写 DAC 寄存器 | SignalOutput.c |
| `StartDACWave()` | 启动 DMA 波形输出。配置 DMA2 Ch3 + 使能 DAC DMA | SignalOutput.c |
| `StopDAC()` | 停止所有输出。关 DMA + DAC 归零 | SignalOutput.c, Main.c |
| `SetDACWave(StructDACWave wave)` | 设置波形缓冲区描述符（地址+长度） | SetDACWaveTri/SetDACWaveSine, ProcHostCmd.c |
| `SetDACWaveTri()` | 切换到三角波（更新波形缓冲区指针） | SignalOutput.c |
| `SetDACWaveSine()` | 切换到正弦波 | SignalOutput.c |

### Wave.c

| 函数 | 做什么 |
|------|--------|
| `InitWave()` | 初始化波形数据（可为空函数） |
| `GetTriWave100PointAddr()` | 返回 100 点三角波数组地址 |
| `GetSineWave100PointAddr()` | 返回 100 点正弦波数组地址 |
| `GetRectWave100PointAddr()` | 返回 100 点方波数组地址 |

### ADC.c

| 函数 | 做什么 | 谁调用 |
|------|--------|--------|
| `InitADC()` | 初始化 PA1、ADC1、DMA1 Ch1。连续转换 + 循环 DMA | Main.c |
| `GetADCBuf()` | 返回 128 点采样缓冲区指针 | SignalMeasure.c |
| `GetADCStats(&avg,&max,&min,&p2p)` | 计算 128 点的平均/最大/最小/峰峰值 | SignalMeasure.c, Main.c |
| `GetADCLatest()` | 返回最新一个采样值 | 可选 |

## 关键参数

### DAC 波形输出

```
TIM4: PSC=71, ARR=99
  -> 计数频率 = 72MHz / 72 = 1MHz
  -> 触发频率 = 1MHz / 100 = 10kHz (采样率)
  -> 100点波形 -> 输出频率 = 10kHz / 100 = 100Hz

DAC 数据寄存器地址: 0x40007408 (DAC_DHR12R1)
电压转换: dacValue = voltage_V * 4095 / 3.3
  1.0V -> 1241, 2.0V -> 2482
```

### ADC 采集

```
ADC 时钟: PCLK2/6 = 72MHz/6 = 12MHz
采样时间: 239.5 周期
  -> 转换时间 = (239.5 + 12.5) / 12MHz = 21us
  -> 采样率 = 1/21us = ~47.6kHz
DMA: 循环模式, 128 点缓冲区
电压转换: voltage_mV = adcValue * 3300 / 4095
```

### 波形数据生成

```
三角波 (100点, 0~4095):
  前50点: 0, 82, 164, ... 4095 (线性上升, 步长82)
  后50点: 4095, 4013, ... 0 (线性下降)

正弦波 (100点, 0~4095):
  value[i] = 2048 + 2047 * sin(2*PI*i/100)
  Cortex-M3 无 FPU, 建议预计算或用整数近似

方波 (100点, 0/4095):
  前50点: 0, 后50点: 4095
```

## 实现建议

1. **先做 Wave.c**：生成三个波形数组，写好 getter 函数
2. **再做 DAC.c**：先实现 DC 输出 (StartDAC)，用万用表验证电压
3. **然后 DAC 波形**：实现 DMA 配置，用示波器在 PA4 验证波形
4. **最后 ADC.c**：实现采集和统计，用信号发生器输入 PA1 验证

## 测试方法

- **DAC DC**: 万用表测 PA4，StartDAC(1241) 应输出 ~1.0V
- **DAC 波形**: 示波器接 PA4，应看到 100Hz 三角波/正弦波
- **ADC**: 信号发生器接 PA1（0~3.3V），OLED 显示电压应与输入匹配
- **注意**: ADC 输入不能超过 3.3V 或为负电压！
