#include "Main.h"
#include "PackUnpack.h"
#include "SendDataToHost.h"
#include "ProcHostCmd.h"
#include "SignalOutput.h"




static  void  InitSoftware(void);
static  void  InitHardware(void);
static  void  Proc2msTask(void);
static  void  Proc1SecTask(void);

static  void  InitSoftware(void)
{
  InitPackUnpack();
  InitSendDataToHost();
  InitProcHostCmd();
}

/*
 * 初始化所有硬件外设。
 *
 * 调用顺序很重要！关键点：
 *   - SystemInit() 必须最先调用（设置 72MHz 时钟）
 *   - InitSettings() 必须在 InitSignalOutput() 之前调用
 *     （InitSignalOutput 会从 Settings 读取 Flash 中加载的模式）
 *   - InitOLED() 必须在 InitDisplay() 之前（显示功能需要 OLED）
 *   - InitIWDG(独立看门狗) 应靠后调用（系统稳定后再启动看门狗）
 *
 * 建议顺序：
 *   SystemInit, InitRCC, InitNVIC, InitUART1(115200), InitTimer,
 *   InitLED, InitSysTick, InitKeyOne, InitProcKeyOne, InitIWDG,
 *   InitADC, InitDAC, InitOLED, InitDisplay, InitPWM, InitCapture, InitFlash
 *
 * 注意：InitDisplay() 内部会依次调用 InitSignalMeasure() -> InitSettings()
 *       -> InitSignalOutput()。InitOLED 必须在 InitDisplay 之前调用，
 *       以便任何初始化函数需要显示诊断信息时 OLED 已就绪。
 */
static  void  InitHardware(void)
{
  SystemInit();       // 系统时钟 72MHz
  InitRCC();
  InitNVIC();
  InitUART1(115200);
  InitTimer();
  InitLED();
  InitSysTick();
  InitKeyOne();
  InitProcKeyOne();
  InitIWDG();         // 看门狗靠后启动，系统稳定后再喂

  InitADC();          // A组：ADC 连续采样 + DMA
  InitDAC();          // A组：DAC 输出
  InitOLED();         // C组：OLED（须在 InitDisplay 之前）
  InitDisplay();      // D组：内部依次 InitSignalMeasure -> InitSettings -> InitSignalOutput
  InitPWM();          // B组：PWM 输出 TIM3_CH2 -> PB5（部分重映射）
  InitCapture();      // B组：输入捕获 TIM1_CH1 <- PA8（双沿，测频率+占空比）
  InitFlash();        // C组：Flash 读写
}

/*
 * 2ms 周期任务 - 系统心跳。
 *
 * 仅在 Get2msFlag() 返回真时执行（由定时器中断服务程序置位）。
 *
 * 每 2ms 执行的任务：
 *   1. 扫描全部 3 个按键：ScanKeyOne(KEY_NAME_KEY1, ProcKeyUpKey1, ProcKeyDownKey1)
 *                         ScanKeyOne(KEY_NAME_KEY2, ProcKeyUpKey2, ProcKeyDownKey2)
 *                         ScanKeyOne(KEY_NAME_KEY3, ProcKeyUpKey3, ProcKeyDownKey3)
 *   2. ProcKeyCheckKey3()  - KEY3 长按检测
 *   3. CaptureTimeoutCheck() - 输入捕获超时检查（来自 Capture.c）
 *   4. DisplayProcess()     - 显示刷新（内部限频到 10Hz）
 *   5. 清除标志：Clr2msFlag()
 *
 * 可选功能：KEY1+KEY2 同时长按 3 秒 -> while(1) 故障陷阱
 *           （计数 s_faultCnt，若 >= 1500 则进入死循环）
 */
static  void  Proc2msTask(void)
{
  static u16 s_faultCnt = 0;   // KEY1+KEY2 同时按住计数（*2ms）

  if(Get2msFlag())  // 仅 2ms 标志置位时执行
  {
    // 1. 扫描 3 个按键（按下/释放回调由 KeyOne 框架分发）
    ScanKeyOne(KEY_NAME_KEY1, ProcKeyUpKey1, ProcKeyDownKey1);
    ScanKeyOne(KEY_NAME_KEY2, ProcKeyUpKey2, ProcKeyDownKey2);
    ScanKeyOne(KEY_NAME_KEY3, ProcKeyUpKey3, ProcKeyDownKey3);

    // 2. KEY3 长按检测（达到 2 秒置 longPress，释放时触发 SaveSettings）
    ProcKeyCheckKey3();

    // 3. 输入捕获无信号超时检测（每 2ms 调用，1s 无信号清零）
    CaptureTimeoutCheck();

    // 4. 显示刷新（内部限频到 10Hz，分发到当前页面）
    DisplayProcess();

    // 5. 故障陷阱（可选）：KEY1 + KEY2 同时按住 3 秒 -> 死循环
    if(IsKeyPressed(KEY_NAME_KEY1) && IsKeyPressed(KEY_NAME_KEY2))
    {
      s_faultCnt++;
      if(s_faultCnt >= 1500)   // 1500 * 2ms = 3000ms = 3s
      {
        while(1);
      }
    }
    else
    {
      s_faultCnt = 0;
    }

    Clr2msFlag();   // 清除 2ms 标志
  }
}


/*
 * 1 秒周期任务 - USART 状态输出。
 *
 * 仅在 Get1SecFlag() 返回真时执行。
 *
 * 输出内容取决于当前显示页面：
 *   信号输出：  printf("Mode:%d\r\n", (u8)GetSignalOutputType());
 *   信号测量：  printf("ADC:%dmV Freq:%dHz\r\n", voltage_mV, freq);
 *               （voltage = avg*3300/4096, freq = GetCaptureFreq()）
 *   设置：      printf("Settings\r\n");
 *
 * 最后调用 PrintState() 和 Clr1SecFlag()。
 */
static  void  Proc1SecTask(void)
{
  if(Get1SecFlag())  // 仅 1s 标志置位时执行
  {
    switch(GetDisplayState())
    {
      case DISPLAY_SIGNAL_OUTPUT:
        printf("Mode:%d\r\n", (u8)GetSignalOutputType());
        break;

      case DISPLAY_SIGNAL_MEASURE:
      {
        u16 avg, maxV, minV, p2p;
        u32 voltage_mV;
        u32 freq;
        GetADCStats(&avg, &maxV, &minV, &p2p);   // A组：128 点统计
        (void)maxV; (void)minV; (void)p2p;       // 1s 任务仅用 avg，其余抑制未用警告
        voltage_mV = (u32)avg * 3300 / 4095;     // 12 位 ADC，3.3V 参考
        freq = GetCaptureFreq();                  // B组：捕获频率，0 = 无信号
        printf("ADC:%dmV Freq:%dHz\r\n", (u16)voltage_mV, (u32)freq);
        break;
      }

      case DISPLAY_SETTINGS:
        printf("Settings\r\n");
        break;

      default:
        break;
    }

    PrintState();
    Clr1SecFlag();   // 清除 1s 标志
  }
}


int main(void)
{
  InitSoftware();
  InitHardware();

  /* 启动时确保输出已停止 */
  StopDAC();
  StopPWM();

  while(1)
  {
    Proc2msTask();
    Proc1SecTask();
    FeedIWDG();
  }
}
