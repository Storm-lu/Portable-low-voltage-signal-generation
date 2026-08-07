#include "ProcKeyOne.h"
#include "UART1.h"
#include "IWDG.h"
#include "Display.h"
#include "SignalOutput.h"
#include "Settings.h"

void InitProcKeyOne(void)
{
  /* Nothing to initialize currently. */
}


/*
 * KEY1 down callback.
 *
 * Behavior depends on current display page:
 *   Signal Output page -> toggle output start/stop (call OutputChange())
 *   Settings page      -> move cursor to next setting item (call ParamChange())
 *   Signal Measure page-> no action
 *
 * Use GetDisplayState() to check which page is active.
 */
void  ProcKeyDownKey1(void)
{
  /* TODO: Check GetDisplayState() and dispatch accordingly. */
}


void  ProcKeyUpKey1(void)
{
  /* Currently no action on KEY1 release. */
}


/*
 * KEY2 down callback.
 *
 * Behavior:
 *   Settings page -> increment current setting value (call ValueChange())
 *   Other pages   -> no action
 */
void  ProcKeyDownKey2(void)
{
  /* TODO: If in Settings page, call ValueChange(). */
}


void  ProcKeyUpKey2(void)
{
  /* Currently no action on KEY2 release. */
}


/*
 * Long-press detection state variables.
 *
 * d_cnt     - counts 2ms ticks while KEY3 is held (1000 ticks = 2 seconds)
 * holding   - 1 while KEY3 is being held down (for long-press timing)
 * longPress - 1 when 2s threshold reached, cleared on release
 *
 * These are used by ProcKeyDownKey3, ProcKeyUpKey3, and ProcKeyCheckKey3.
 */
static int d_cnt = 0;
static int holding = 0, longPress = 0;


/*
 * KEY3 down callback.
 *
 * Behavior:
 *   Settings page      -> start long-press timer (set holding=1, longPress=0, d_cnt=0)
 *   Other pages        -> short press: switch to next page (call DisplayChange())
 *
 * The long-press timer is checked by ProcKeyCheckKey3() every 2ms.
 */
void  ProcKeyDownKey3(void)
{
  /* TODO:
   *   Reset d_cnt=0, longPress=0.
   *   If in Settings page: holding=1 (start long-press timing).
   *   Else: holding=0, call DisplayChange() (short press page switch). */
}


/*
 * KEY3 up callback.
 *
 * Behavior:
 *   If longPress==1 (was held 2s+):
 *     In Settings page -> save settings (call SaveSettings())
 *     In other pages   -> switch page (call DisplayChange())
 *   If longPress==0 (short press):
 *     This case was already handled in ProcKeyDownKey3 for non-Settings pages.
 *     For Settings page, a short press does nothing (no page switch in Settings).
 *
 * Always reset holding=0 on release.
 */
void  ProcKeyUpKey3(void)
{
  /* TODO:
   *   holding = 0.
   *   If longPress == 1:
   *     longPress = 0, d_cnt = 0.
   *     If in Settings: call SaveSettings().
   *     Else: call DisplayChange().
   */
}


/*
 * KEY3 long-press checker - called every 2ms from Proc2msTask().
 *
 * If holding==1, increment d_cnt.
 * When d_cnt >= 1000 (1000 * 2ms = 2 seconds), set longPress=1 and holding=0.
 *
 * After this, when KEY3 is released, ProcKeyUpKey3 will see longPress==1
 * and trigger SaveSettings().
 */
void ProcKeyCheckKey3(void)
{
  /* TODO:
   *   If holding == 1:
   *     d_cnt++
   *     If d_cnt >= 1000: longPress = 1, holding = 0
   */
}
