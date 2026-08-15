# Team D - 应用整合层 (Main + 三页面)

## 你负责的文件

| 文件 | 作用 | 状态 |
|------|------|------|
| `App/Main/Main.c` | 系统初始化、USART、IWDG 看门狗、2ms/1s 任务调度 | **需实现** |
| `App/Main/Main.h` | 全局头文件包含（**需补全 include**） | 需补全 |
| `App/Main/Display.c` | 三页面状态机、10Hz 刷新控制 | **需实现** |
| `App/Main/Display.h` | DisplayState 枚举、接口声明（已加文档注释） | 已完成 |
| `App/Main/SignalOutput.c` | 信号输出页：模式显示、Start/Stop、模式切换 | **需实现** |
| `App/Main/SignalOutput.h` | 枚举、接口声明（已加文档注释） | 已完成 |
| `App/Main/SignalMeasure.c` | 信号测量页：波形显示、电压/频率/占空比 | **需实现** |
| `App/Main/SignalMeasure.h` | 测量页接口（已加文档注释） | 已完成 |
| `App/Main/Settings.c` | 参数设置页：模式/频率/占空比选择、Flash 保存 | **需实现** |
| `App/Main/Settings.h` | 枚举、getter 声明（已加文档注释） | 已完成 |

## 代码掏空情况

你的代码**已被掏空**，和 A/B/C 一样需要从头实现。保留的内容：

**Main.c 保留了：**
- `InitSoftware()` 已实现（不需要改）
- `main()` 函数骨架已实现（不需要改）

**Main.c 需要你实现：**
- `InitHardware()` — 按正确顺序调用所有 init 函数
- `Proc2msTask()` — 2ms 心跳任务（按键扫描 + 捕获超时 + 显示刷新）
- `Proc1SecTask()` — 1秒任务（USART 状态输出）

**Display.c 保留了：**
- `PrintState()` 已实现
- `GetDisplayState()` 已实现
- `displayState` 和 `s_displayCnt` 变量声明

**Display.c 需要你实现：**
- `InitDisplay()` — 初始化三页面（顺序：SignalMeasure → Settings → SignalOutput）
- `DisplayProcess()` — 10Hz 降频 + 状态机分发
- `DisplayChange()` — 页面循环切换

**SignalOutput.c 保留了：**
- `GetSignalOutputType()` 已实现
- `signal_mode` 和 `signal_state` 变量声明

**SignalOutput.c 需要你实现：**
- `InitSignalOutput()` — 从 Settings 读取模式
- `DisplaySignalOutput()` — 输出页布局
- `OutputChange()` — Start/Stop 切换
- `SetSignalOutputType()` — 模式切换 + 重启输出

**SignalMeasure.c 保留了：**
- `InitSignalMeasure()` 已实现（空函数）
- 波形区域宏定义

**SignalMeasure.c 需要你实现：**
- `DisplaySignalMeasure()` — 测量页布局（波形 + 右侧文本参数）

**Settings.c 保留了：**
- 3 个 getter 函数已实现（**不要修改**）
- 静态变量和宏定义

**Settings.c 需要你实现：**
- `InitSettings()` — 从 Flash 读取参数 + 验证
- `DisplaySettings()` — 设置页布局
- `ParamChange()` — 光标移动
- `ValueChange()` — 值递增
- `SaveSettings()` — 保存到 Flash + 应用输出

## 第一步：补全 Main.h

当前 `Main.h` 已删除了所有模块的 include。你需要加回来：

```c
// Team A:
#include "DAC.h"
#include "Wave.h"
#include "ADC.h"
// Team B:
#include "PWM.h"
#include "Capture.h"
// Team C:
#include "OLED.h"
#include "ProcKeyOne.h"
#include "Flash.h"
// Team D (your own):
#include "Display.h"
#include "SignalOutput.h"
#include "SignalMeasure.h"
#include "Settings.h"
```

**注意**：在所有人完成实现之前，项目无法编译，这是正常的。

## 实现优先级

1. **Main.h** — 先把 include 加上，让编译器能看到所有接口
2. **Display.c** — 状态机骨架（InitDisplay + DisplayProcess + DisplayChange）
3. **Settings.c** — InitSettings（Flash 读取）+ getter（已有）
4. **SignalOutput.c** — InitSignalOutput + DisplaySignalOutput + OutputChange
5. **SignalMeasure.c** — DisplaySignalMeasure（依赖 A/B 的接口）
6. **Main.c** — InitHardware + Proc2msTask + Proc1SecTask
7. **Settings.c** — DisplaySettings + ParamChange + ValueChange + SaveSettings
8. **SignalOutput.c** — SetSignalOutputType

## 关键设计要点

### 初始化顺序（InitHardware + InitDisplay）

```
InitHardware():
  系统级: SystemInit → InitRCC → InitNVIC → InitUART1 → InitTimer
          → InitLED → InitSysTick → InitKeyOne → InitProcKeyOne → InitIWDG
  外设级: InitADC → InitDAC → InitOLED → InitDisplay → InitPWM → InitCapture → InitFlash

InitDisplay() 内部:
  InitSignalMeasure() → InitSettings() → InitSignalOutput()
                                    ↑                ↑
                              从Flash读取       读取Settings的模式
```

**为什么 InitSettings 必须在 InitSignalOutput 之前？**
因为 `InitSignalOutput()` 调用 `GetSettingsSignalMode()` 读取 Settings 中的模式。
如果 Settings 还没从 Flash 加载，SignalOutput 就会用默认值 DC_1V。

### 2ms 任务（Proc2msTask）

```
if(Get2msFlag()) {
    1. 三个按键扫描: ScanKeyOne × 3
    2. ProcKeyCheckKey3()         — C 的长按检测
    3. CaptureTimeoutCheck()      — B 的捕获超时
    4. DisplayProcess()           — D 的显示刷新（内部10Hz降频）
    5. (可选) KEY1+KEY2同时按3秒 → while(1) 故障陷阱
    6. Clr2msFlag()
}
```

### DisplayProcess 降频

```c
s_displayCnt++;
if(s_displayCnt < 50) return;  // 50 × 2ms = 100ms = 10Hz
s_displayCnt = 0;
// 切换到当前页面的 Display 函数
```

### DC 电压对应的 DAC 值

```
12位DAC, 参考电压3.3V
DC 1V:  1.0 / 3.3 × 4095 ≈ 1241
DC 2V:  2.0 / 3.3 × 4095 ≈ 2482
```

### 信号测量页布局

```
+--- 波形区 (80×48) ---+-- 文本区 (48px) --+
|                      | V:x.xV    y=0    |  峰值电压(maxV)
|  网格 + 自动缩放波形  | F:xxxH    y=16   |  频率
|                      | P:x.xV    y=32   |  峰峰值(p2p)
+----------------------+ D:xx%     y=48   |  占空比(仅有效信号)
```

电压转换：`mV = ADC_value × 3300 / 4095`

### 设置页光标逻辑

- 非 PWM 模式：只显示 Mode 行，光标锁定在 Mode
- PWM 模式：显示 Mode + Freq + Duty 三行，光标可循环切换
- 从 PWM 切到非 PWM 时：光标重置回 Mode

## 联调要点

1. **A 交付后**：测试 DAC 输出页（DC/三角波/正弦波），测试 ADC 测量页波形
2. **B 交付后**：测试 PWM 输出，测试 Capture 频率/占空比显示
3. **C 交付后**：测试 OLED 显示，测试按键切换，测试 Flash 保存/加载
4. **全部交付后**：完整流程测试（设置→保存→重启→验证加载）
5. **常见问题**：
   - 忘记在 Main.h 加 include → 编译报错
   - 初始化顺序错误 → SignalOutput 读不到 Flash 数据
   - 忘记 FeedIWDG() → 看门狗复位
   - DisplayProcess 没降频 → OLED 闪烁
