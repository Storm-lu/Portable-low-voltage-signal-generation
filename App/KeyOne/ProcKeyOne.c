#include "ProcKeyOne.h"
#include "UART1.h"
#include "IWDG.h"
#include "Display.h"
#include "SignalOutput.h"
#include "Settings.h"

void InitProcKeyOne(void)
{
  /* 当前无需初始化。 */
}


/*
 * KEY1 按下回调。
 *
 * 行为取决于当前显示页面：
 *   信号输出页 -> 切换输出启动/停止（调用 OutputChange()）
 *   设置页     -> 光标移到下一个设置项（调用 ParamChange()）
 *   信号测量页 -> 无操作
 *
 * 使用 GetDisplayState() 检查当前处于哪个页面。
 */
void  ProcKeyDownKey1(void)
{
  /* 检查 GetDisplayState() 并相应地分发处理。 */
  switch (GetDisplayState())
  {
  //信号输出页 -> 切换输出启动/停止
  case DISPLAY_SIGNAL_OUTPUT:
    OutputChange();
    break;
  //设置页 -> 光标移到下一个设置项
  case DISPLAY_SETTINGS:
    ParamChange();
    break;
  case DISPLAY_SIGNAL_MEASURE:
    // 信号测量页按下 KEY1 无操作
  default:
    break;
  }
}


void  ProcKeyUpKey1(void)
{
  /* 当前 KEY1 释放时无操作。 */
}


/*
 * KEY2 按下回调。
 *
 * 行为：
 *   设置页 -> 当前设置值递增（调用 ValueChange()）
 *   其他页 -> 无操作
 */
void  ProcKeyDownKey2(void)
{
  /* 若处于设置页，调用 ValueChange()。 */
  if (GetDisplayState() == DISPLAY_SETTINGS)
  {
    ValueChange();
  }
}


void  ProcKeyUpKey2(void)
{
  /* 当前 KEY2 释放时无操作。 */
}


/*
 * 长按检测状态变量。
 *
 * d_cnt     - KEY3 按住期间的2ms滴答计数（1000次 = 2秒）
 * holding   - KEY3 正在被按住时为1（用于长按计时）
 * longPress - 达到2秒阈值时置1，释放时清零
 *
 * 这些变量由 ProcKeyDownKey3、ProcKeyUpKey3 和 ProcKeyCheckKey3 使用。
 */
static int d_cnt = 0;
static int holding = 0, longPress = 0;


/*
 * KEY3 按下回调。
 *
 * 行为：
 *   设置页 -> 启动长按计时器（置 holding=1, longPress=0, d_cnt=0）
 *   其他页 -> 短按：切换到下一页（调用 DisplayChange()）
 *
 * 长按计时器由 ProcKeyCheckKey3() 每2ms检查一次。
 */
void  ProcKeyDownKey3(void)
{
  /*   重置 d_cnt=0, longPress=0。
   *   若处于设置页：holding=1（启动长按计时）。
   *   否则：holding=0，调用 DisplayChange()（短按切页）。 */
  d_cnt = 0;
  longPress = 0;

  if (GetDisplayState() == DISPLAY_SETTINGS)
  {
    holding = 1; // 启动长按计时
  }
  else
  {
    holding = 0; // 非设置页，短按切页
    DisplayChange();
  }
  
}


/*
 * KEY3 释放回调。
 *
 * 行为：
 *   若 longPress==1（曾被按住2秒以上）：
 *     在设置页 -> 保存设置（调用 SaveSettings()）
 *     在其他页 -> 切换页面（调用 DisplayChange()）
 *   若 longPress==0（短按）：
 *     非设置页的情况已在 ProcKeyDownKey3 中处理。
 *     对于设置页，短按无操作（设置页中不切页）。
 *
 * 释放时始终重置 holding=0。
 */
void  ProcKeyUpKey3(void)
{
  /*   holding = 0。
   *   若 longPress == 1：
   *     longPress = 0, d_cnt = 0。
   *     若处于设置页：调用 SaveSettings()。
   *     否则：调用 DisplayChange()。
   */

  holding = 0; // 停止长按计时

  if (longPress == 1)
  {
    longPress = 0;
    d_cnt = 0;

    if (GetDisplayState() == DISPLAY_SETTINGS)
    {
      SaveSettings();
    }
    else
    {
      DisplayChange();
    }
  }
   
}


/*
 * KEY3 长按检测函数 - 由 Proc2msTask() 每2ms调用一次。
 *
 * 若 holding==1，递增 d_cnt。
 * 当 d_cnt >= 1000（1000 * 2ms = 2秒）时，置 longPress=1 且 holding=0。
 *
 * 此后当 KEY3 被释放时，ProcKeyUpKey3 会检测到 longPress==1
 * 并触发 SaveSettings()。
 */
void ProcKeyCheckKey3(void)
{
  /*   若 holding == 1：
   *     d_cnt++
   *     若 d_cnt >= 1000：longPress = 1, holding = 0
   */

  if (holding == 1)
  {
    d_cnt++;
    if (d_cnt >= 1000)
    {
      longPress = 1;
      holding = 0; // 停止计时，避免重复触发
    }
  }

}
