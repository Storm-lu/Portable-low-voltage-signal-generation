# Team C - 外设驱动层 (OLED + Key + Flash)

## 你负责的文件

| 文件 | 作用 | 状态 |
|------|------|------|
| `App/OLED/OLED.c` | OLED 显示驱动：软件 SPI、GRAM 缓冲、字符/数字/波形绘制 | **需实现** |
| `App/OLED/OLED.h` | 显示接口声明（已加文档注释） | 已完成 |
| `App/KeyOne/ProcKeyOne.c` | 按键逻辑处理：短按/长按检测、页面切换、功能回调 | **需实现** |
| `App/KeyOne/ProcKeyOne.h` | 按键回调声明（已加文档注释） | 已完成 |
| `HW/Flash/Flash.c` | Flash 读写驱动：页擦除+重写 | **需实现** |
| `HW/Flash/Flash.h` | Flash 接口声明（已加文档注释） | 已完成 |

## 代码掏空情况

你的代码**已被掏空**，和 A/B 一样需要从头实现。保留的内容：

**OLED.c 保留了：**
- 引脚宏定义（CLR/SET 宏）
- GRAM 数组声明 `s_arrOLEDGRAM[128][8]`
- 函数原型和 static 声明

**OLED.c 需要你实现：**
- `ConfigOLEDGPIO()` — 配置 PB12/13/14/15, PC3 为推挽输出
- `ConfigOLEDReg()` — SSD1306 初始化命令序列
- `OLEDWriteByte()` — 软件 SPI 逐位发送
- `OLEDDrawPoint()` — GRAM 画点（核心，其他绘制函数依赖它）
- `CalcPow()` — 整数幂运算
- `InitOLED()` — 初始化流程
- `OLEDDisplayOn/Off()` — 开关显示
- `OLEDRefreshGRAM()` — GRAM 刷新到屏幕
- `OLEDClear()` — 清屏
- `OLEDShowChar()` — 单字符显示
- `OLEDShowNum()` — 数字显示（前导零抑制）
- `OLEDShowString()` — 字符串显示
- `OLEDDrawGrid()` — 虚线网格
- `OLEDDrawWaveFormEx()` — 波形绘制（auto-scale + 连线）

**ProcKeyOne.c 需要你实现：**
- `ProcKeyDownKey1()` — KEY1 按下（Output页=Start/Stop, Settings页=选参数项）
- `ProcKeyDownKey2()` — KEY2 按下（Settings页=递增值）
- `ProcKeyDownKey3()` — KEY3 按下（Settings页=开始长按计时, 其他页=切页）
- `ProcKeyUpKey3()` — KEY3 释放（长按=保存, 短按=切页）
- `ProcKeyCheckKey3()` — 每2ms调用，长按检测

**Flash.c 保留了：**
- `s_arrFlashBuf` 页缓冲数组
- `ReadWord()` 和 `WriteWordNoCheck()` 的函数原型（但实现也掏空了）

**Flash.c 需要你实现：**
- `ReadWord()` — 读取一个32位字（指针解引用）
- `WriteWordNoCheck()` — 直接写入（不检查擦除）
- `STM32FlashWriteWord()` — 页感知写入（读页→检查→擦除→合并→写回）
- `STM32FlashReadWord()` — 连续读取多个字

## 实现建议

### 优先级
1. **先做 OLED 底层**：ConfigOLEDGPIO → OLEDWriteByte → ConfigOLEDReg → OLEDDrawPoint → InitOLED
2. **再做 OLED 上层**：OLEDRefreshGRAM → OLEDClear → OLEDShowChar → OLEDShowNum → OLEDShowString
3. **然后做 Flash**：ReadWord → STM32FlashReadWord → WriteWordNoCheck → STM32FlashWriteWord
4. **最后做 Key**：ProcKeyDown/Up 各回调函数
5. **OLEDDrawGrid 和 OLEDDrawWaveFormEx 最后做**（只影响波形显示，其他页面不依赖）

### 关键提示

**OLED GRAM 机制：**
- 所有绘制函数只写内存数组 `s_arrOLEDGRAM[128][8]`，不直接操作屏幕
- 只有 `OLEDRefreshGRAM()` 才把数据发送到 OLED
- 每个 Display 函数的模式：`OLEDClear() → 画东西 → OLEDRefreshGRAM()`

**GRAM 画点的坐标映射：**
```
y=0 → page=7, bit=7  (y/8=0, page=7-0=7, bit=1<<(7-0)=0x80)
y=1 → page=7, bit=6  (y/8=0, page=7-0=7, bit=1<<(7-1)=0x40)
...
y=8 → page=6, bit=7  (y/8=1, page=7-1=6, bit=1<<(7-0)=0x80)
```

**Flash 写入流程（STM32FlashWriteWord）：**
```
1. FLASH_Unlock()
2. 计算目标地址在页内的偏移
3. 读出整页内容到 s_arrFlashBuf
4. 检查目标位置是否全为 0xFF（已擦除）
5. 如果不是 0xFF：
   - FLASH_ErasePage() 擦除整页
   - 把新数据合并到 s_arrFlashBuf
   - WriteWordNoCheck 写回整页
6. 如果是 0xFF：
   - 直接 WriteWordNoCheck 写入
7. FLASH_Lock()
```

**长按检测：**
- `ProcKeyCheckKey3()` 每 2ms 被调用一次
- `d_cnt` 从 0 开始递增
- `d_cnt >= 1000` 时，1000 × 2ms = 2秒，触发长按

## 与其他人的接口

你提供的接口被 D 调用：
```
D -> C (OLED):  OLEDClear / OLEDShowString / OLEDShowNum / OLEDShowChar
                OLEDDrawGrid / OLEDDrawWaveFormEx / OLEDRefreshGRAM
D -> C (Key):   ProcKeyDownKey1/2/3, ProcKeyUpKey1/2/3, ProcKeyCheckKey3
D -> C (Flash): STM32FlashReadWord / STM32FlashWriteWord
```

你调用的 D 的函数（ProcKeyOne.c 中）：
```
C -> D:  GetDisplayState()  (判断当前页面)
         OutputChange()     (KEY1 在输出页)
         ParamChange()      (KEY1 在设置页)
         ValueChange()      (KEY2 在设置页)
         SaveSettings()     (KEY3 长按)
         DisplayChange()    (KEY3 短按切页)
```

**注意**：ProcKeyOne.c 需要调用 D 的函数，但 D 的实现也是掏空的。
两人在 Phase 1 先约定好函数签名（.h 已经定义好了），各自独立实现。
