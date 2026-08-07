#include "Main.h"
#include "Display.h"
#include "SignalMeasure.h"
#include "SignalOutput.h"
#include "Settings.h"

DisplayState displayState;
static u8 s_displayCnt = 0;

/*
 * 初始化显示系统。
 *
 * 步骤：
 *   1. 将初始页面设为 DISPLAY_SIGNAL_MEASURE
 *   2. 复位帧计数器 s_displayCnt = 0
 *   3. 调用 InitSignalMeasure()
 *   4. 调用 InitSettings()       -- 从 Flash 加载参数（必须在 SignalOutput 之前）
 *   5. 调用 InitSignalOutput()   -- 通过 GetSettingsSignalMode() 读取模式
 *
 * 顺序很关键：Settings 必须先从 Flash 加载，SignalOutput 才能读到模式。
 */
void InitDisplay(void)
{
    /* 待实现：按上述说明实现。 */
}

/*
 * 显示处理函数 - 由 Proc2msTask 每 2ms 调用。
 *
 * 限频：每 50 次调用刷新一次（50 * 2ms = 100ms = 10Hz）。
 * 防止刷新过于频繁导致 OLED 闪烁。
 *
 * 当计数器达到 50 时，复位为 0，并根据当前 displayState
 * 调用对应的显示函数：
 *   DISPLAY_SIGNAL_MEASURE -> DisplaySignalMeasure()
 *   DISPLAY_SIGNAL_OUTPUT  -> DisplaySignalOutput()
 *   DISPLAY_SETTINGS       -> DisplaySettings()
 */
void DisplayProcess(void){
    /* 待实现：
     *   s_displayCnt++;
     *   if(s_displayCnt < 50) return;
     *   s_displayCnt = 0;
     *   switch(displayState) { ... 调用对应的显示函数 ... } */
}

/*
 * 切换到下一个显示页面（循环切换）。
 *
 * 顺序：信号测量 -> 信号输出 -> 设置 -> 信号测量
 *
 * 注意：在设置页面时，KEY3 短按不会调用此函数。
 * 只有长按（2秒）通过 SaveSettings() 退出设置页面。
 */
void DisplayChange(void){
    /* 待实现：根据 displayState 循环切换到下一个页面。 */
}

/*
 * 通过 USART 打印当前显示状态名称（用于调试）。
 * 由 Proc1SecTask() 调用。
 */
void PrintState(void){
    switch(displayState){
        case DISPLAY_SIGNAL_MEASURE:
            printf("Signal Measure\r\n");
            break;
        case DISPLAY_SIGNAL_OUTPUT:
            printf("Signal Output\r\n");
            break;
        case DISPLAY_SETTINGS:
            printf("Settings\r\n");
            break;
    }
}

/*
 * 获取当前显示状态。
 * 供 ProcKeyOne.c 使用，以决定执行哪个按键动作。
 */
DisplayState GetDisplayState(void)
{
  return displayState;
}
