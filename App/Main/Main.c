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
 * Initialize all hardware peripherals.
 *
 * Call order matters! Key points:
 *   - SystemInit() must come first (sets up 72MHz clock)
 *   - InitSettings() must be called before InitSignalOutput()
 *     (InitSignalOutput reads Flash-loaded mode from Settings)
 *   - InitOLED() before InitDisplay() (Display functions need OLED)
 *   - InitIWDG() should be late (start watchdog after system is stable)
 *
 * Suggested order:
 *   SystemInit, InitRCC, InitNVIC, InitUART1(115200), InitTimer,
 *   InitLED, InitSysTick, InitKeyOne, InitProcKeyOne, InitIWDG,
 *   InitADC, InitDAC, InitOLED, InitDisplay, InitPWM, InitCapture, InitFlash
 *
 * Note: InitDisplay() internally calls InitSignalMeasure() -> InitSettings()
 *       -> InitSignalOutput(). InitOLED must come before InitDisplay so that
 *       OLED is ready if any init function needs to display diagnostics.
 */
static  void  InitHardware(void)
{
  /* TODO: Call all init functions in the correct order.
   *       Don't forget to include the headers in Main.h first!
   *       The functions come from different team members:
   *         System-level: SystemInit, InitRCC, InitNVIC, InitUART1, InitTimer,
   *                       InitLED, InitSysTick, InitKeyOne, InitIWDG
   *         Team A:       InitADC, InitDAC
   *         Team B:       InitPWM, InitCapture
   *         Team C:       InitOLED, InitFlash, InitProcKeyOne
   *         Team D:       InitDisplay (this calls InitSignalMeasure + InitSettings + InitSignalOutput) */
}

/*
 * 2ms periodic task - the system heartbeat.
 *
 * Only executes when Get2msFlag() returns true (set by Timer ISR).
 *
 * Tasks to run every 2ms:
 *   1. Scan all 3 keys: ScanKeyOne(KEY_NAME_KEY1, ProcKeyUpKey1, ProcKeyDownKey1)
 *                       ScanKeyOne(KEY_NAME_KEY2, ProcKeyUpKey2, ProcKeyDownKey2)
 *                       ScanKeyOne(KEY_NAME_KEY3, ProcKeyUpKey3, ProcKeyDownKey3)
 *   2. ProcKeyCheckKey3()  - KEY3 long-press detection
 *   3. CaptureTimeoutCheck() - input capture timeout (from Capture.c)
 *   4. DisplayProcess()     - display refresh (internally throttled to 10Hz)
 *   5. Clear flag: Clr2msFlag()
 *
 * Optional: KEY1+KEY2 simultaneous hold for 3s -> while(1) fault trap
 *           (counts s_faultCnt, if >= 1500 enter infinite loop)
 */
static  void  Proc2msTask(void)
{
  /* TODO: Implement as described above.
   *       Use static u16 s_faultCnt for the key combo trap. */
}


/*
 * 1-second periodic task - USART status output.
 *
 * Only executes when Get1SecFlag() returns true.
 *
 * Output depends on current display page:
 *   Signal Output:  printf("Mode:%d\r\n", (u8)GetSignalOutputType());
 *   Signal Measure: printf("ADC:%dmV Freq:%dHz\r\n", voltage_mV, freq);
 *                   (voltage = avg*3300/4096, freq = GetCaptureFreq())
 *   Settings:       printf("Settings\r\n");
 *
 * End with PrintState() and Clr1SecFlag().
 */
static  void  Proc1SecTask(void)
{
  /* TODO: Check Get1SecFlag(), switch on GetDisplayState(),
   *       print appropriate status, call PrintState(), Clr1SecFlag(). */
}


int main(void)
{
  InitSoftware();
  InitHardware();

  /* Ensure outputs are stopped at startup */
  StopDAC();
  StopPWM();

  while(1)
  {
    Proc2msTask();
    Proc1SecTask();
    FeedIWDG();
  }
}
