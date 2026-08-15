#include "Main.h"
#include "PackUnpack.h"
#include "SendDataToHost.h"
#include "ProcHostCmd.h"
#include "SignalOutput.h"
#include "DAC.h"
#include "PWM.h"
#include "Display.h"
#include "KeyOne.h"
#include "Capture.h"
#include "ProcKeyOne.h"
#include "UART1.h"
#include "Flash.h"
#include "ADC.h"
#include "OLED.h"


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
  /* 待实现：按正确顺序调用所有初始化函数。
   *       注意先在 Main.h 中包含对应的头文件！
   *       这些函数由不同团队成员提供：
   *         系统级：SystemInit, InitRCC, InitNVIC, InitUART1, InitTimer,
   *                InitLED, InitSysTick, InitKeyOne, InitIWDG
   *         A组：  InitADC, InitDAC
   *         B组：  InitPWM, InitCapture
   *         C组：  InitOLED, InitFlash, InitProcKeyOne
   *         D组：  InitDisplay（内部调用 InitSignalMeasure + InitSettings + InitSignalOutput） */
  SystemInit();
  InitRCC();
  InitNVIC();
  InitUART1(115200);
  InitTimer();
  InitLED();
  InitSysTick();
  InitKeyOne();
  InitADC();
  InitDAC();
  InitPWM();
  InitCapture();
  InitOLED();
  InitFlash();
  InitProcKeyOne();
  InitDisplay();
  InitIWDG();
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
  /* 按上述说明实现。
   *       组合键陷阱使用 static u16 s_faultCnt 计数。 */
  static u16 s_faultCnt = 0;

  if(Get2msFlag())
  {
    ScanKeyOne(KEY_NAME_KEY1, ProcKeyUpKey1, ProcKeyDownKey1);
    ScanKeyOne(KEY_NAME_KEY2, ProcKeyUpKey2, ProcKeyDownKey2);
    ScanKeyOne(KEY_NAME_KEY3, ProcKeyUpKey3, ProcKeyDownKey3);
    ProcKeyCheckKey3();

    CaptureTimeoutCheck();

    DisplayProcess();

    if(IsKeyPressed(KEY_NAME_KEY1) && IsKeyPressed(KEY_NAME_KEY2)){
      s_faultCnt++;
      if(s_faultCnt >= 1500){
        while(1);
      }
    }else{
      s_faultCnt = 0;
    }

    Clr2msFlag();
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
  /* 待实现：检查 Get1SecFlag()，根据 GetDisplayState() 分支，
   *       打印相应状态，调用 PrintState()、Clr1SecFlag()。 */
  if(Get1SecFlag())
  {
    switch(GetDisplayState()){
      case DISPLAY_SIGNAL_OUTPUT:{
        SignalOutputType mode = GetSignalOutputType();
        printf("Mode:%d\r\n", (u8)mode);
        break;
      }
      case DISPLAY_SIGNAL_MEASURE:{
        u16 avg, maxV, minV, p2p;
        u32 freq;
        GetADCStats(&avg, &maxV, &minV, &p2p);
        freq = GetCaptureFreq();
        printf("ADC:%dmV Freq:%dHz\r\n", (u16)((u32)avg * 3300 / 4096), freq);
        break;  
      }
      case DISPLAY_SETTINGS:
      {
        printf("Settings\r\n");
        break;
      }
    }
    PrintState();
    Clr1SecFlag();
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
