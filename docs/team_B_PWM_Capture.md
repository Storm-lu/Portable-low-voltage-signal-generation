# Team B - 定时器信号链 (PWM + Capture)

## 你负责的文件

| 文件 | 作用 |
|------|------|
| `HW/PWM/PWM.c` | PWM 输出 (TIM3 CH2 -> PB5)，频率/占空比切换 |
| `HW/PWM/PWM.h` | 接口声明（已有文档注释，按此实现） |
| `HW/Capture/Capture.c` | 输入捕获 (TIM1 CH1 <- PA8)，频率/占空比测量 |
| `HW/Capture/Capture.h` | 接口声明（已有文档注释，按此实现） |

## 硬件资源

| 外设 | 用途 | 引脚 |
|------|------|------|
| TIM3 Channel 2 | PWM 输出 | PB5 (部分重映射) |
| TIM1 Channel 1 | 输入捕获 | PA8 |
| NVIC | TIM1 捕获中断 + 溢出中断 | - |

**无冲突**：TIM3 和 TIM1 是不同定时器，PB5 和 PA8 不与其他人冲突。

## 需要实现的函数

### PWM.c

| 函数 | 做什么 | 谁调用 |
|------|--------|--------|
| `InitPWM()` | 初始化 TIM3 CH2 -> PB5，默认 1kHz/50%，CCR=0 不输出 | Main.c |
| `StartPWM(freq, duty)` | 配置定时器并启动 PWM 输出 | SignalOutput.c |
| `StopPWM()` | CCR 归零，关闭定时器 | SignalOutput.c, Main.c |
| `SetPWMFreq(freq)` | 运行时切换频率（改 ARR） | Settings.c |
| `SetPWMDuty(duty)` | 运行时切换占空比（改 CCR） | Settings.c |

### Capture.c

| 函数 | 做什么 | 谁调用 |
|------|--------|--------|
| `InitCapture()` | 初始化 TIM1 CH1 输入捕获，PA8，中断 | Main.c |
| `GetCaptureFreq()` | 返回频率 (Hz)，0=无信号 | SignalMeasure.c, Main.c |
| `GetCaptureDuty()` | 返回占空比 (0~100)，0xFF=无效 | SignalMeasure.c, Main.c |
| `CaptureTimeoutCheck()` | 每 2ms 调用，1s 无信号则清零 | Main.c -> Proc2msTask() |
| `TIM1_CC_IRQHandler()` | 捕获中断，核心测量算法 | 硬件中断 |
| `TIM1_UP_IRQHandler()` | 溢出中断，计数器溢出计数 | 硬件中断 |

## 关键参数

### PWM 输出

```
TIM3: PSC=71 (固定), ARR 可变
  -> 计数频率 = 72MHz / 72 = 1MHz
  -> PWM 频率 = 1MHz / (ARR+1)

频率对照表:
  PWM_FREQ_100Hz -> ARR=9999 -> 1MHz/10000 = 100Hz
  PWM_FREQ_1KHz  -> ARR=999  -> 1MHz/1000  = 1kHz
  PWM_FREQ_5KHz  -> ARR=199  -> 1MHz/200   = 5kHz

占空比计算:
  CCR = (ARR+1) * duty_permille / 1000
  PWM_DUTY_25 -> 250 -> 25%
  PWM_DUTY_50 -> 500 -> 50%
  PWM_DUTY_75 -> 750 -> 75%

GPIO: PB5, AF_PP (复用推挽)
重映射: GPIO_PartialRemap_TIM3 (TIM3_CH2 从默认 PA7 重映射到 PB5)
PWM 模式: PWM mode 2, 极性 Low
```

### 输入捕获

```
TIM1: PSC=71, ARR=65535
  -> 计数频率 = 1MHz (1 count = 1 us)
  -> 最大周期 = 65535 us = 65.5ms (~15Hz 最低频率)

测量算法 (交替沿):
  1. 初始状态: 等待上升沿
  2. 上升沿到来: 记录 CCR, 切换到下降沿
  3. 下降沿到来: 计算高电平时间 -> 占空比, 切换回上升沿
  4. 下一个上升沿: 计算完整周期 -> 频率

周期 = (溢出次数 * 65536 + 当前CCR - 上次CCR) us
频率 = 1000000 / 周期
占空比 = 高电平时间 * 100 / 完整周期

多周期平均: 保存最近 8 次频率测量, 取平均值

切换沿的方法 (重要!):
  标准库没有 TIM_SetIC1Polarity 函数!
  上升沿: TIM1->CCER &= ~TIM_CCER_CC1P;
  下降沿: TIM1->CCER |= TIM_CCER_CC1P;
```

## 实现建议

1. **先做 PWM.c**：比较简单，配置 TIM3 即可
2. **用示波器验证 PWM**：在 PB5 上看到方波，切换频率/占空比
3. **再做 Capture.c**：先实现基本捕获（只测频率），再加占空比
4. **最后加多周期平均**：稳定性提升

## 测试方法

- **PWM**: 示波器接 PB5
  - 100Hz: 时基设 2ms/div
  - 1kHz: 时基设 200us/div
  - 5kHz: 时基设 50us/div
  - 幅度: 0~3.3V

- **Capture**: 用信号发生器或另一块板子的 PWM 输出接到 PA8
  - 输入必须是 0~3.3V 数字信号
  - OLED 测量页应显示正确的频率和占空比

## 难点提示

1. **PWM 部分重映射**：忘记 `GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE)` 会导致 PB5 无输出
2. **捕获沿切换**：必须用寄存器直接操作 CCER，标准库没有对应函数
3. **溢出处理**：低频信号（<15Hz）会导致定时器溢出，必须用 s_ovfCnt 跟踪
4. **中断优先级**：TIM1_CC 和 TIM1_UP 中断的 NVIC 优先级要合理设置
