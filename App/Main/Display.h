#ifndef __DISPLAY_H__
#define __DISPLAY_H__

/*
 * 三页面状态机的显示页面状态。
 * 循环顺序：MEASURE(测量) -> OUTPUT(输出) -> SETTINGS(设置) -> MEASURE
 */
typedef enum
{
  DISPLAY_SIGNAL_MEASURE,   /* ADC 波形 + 频率/占空比显示 */
  DISPLAY_SIGNAL_OUTPUT,    /* 信号模式 + 启动/停止状态 */
  DISPLAY_SETTINGS          /* 参数选择 + Flash 保存 */
} DisplayState;

/* 当前显示页面（extern 供 ProcKeyOne.c 访问）。 */
extern DisplayState displayState;

/*
 * 初始化显示系统。
 * 设置初始页面，调用 InitSignalMeasure、InitSettings、InitSignalOutput。
 * Settings 必须在 SignalOutput 之前初始化（需要 Flash 数据）。
 */
void InitDisplay(void);

/*
 * 显示处理函数 - 每 2ms 调用。
 * 内部限频到 10Hz（每 50 次调用）以防止闪烁。
 * 分发到对应的页面显示函数。
 */
void DisplayProcess(void);

/*
 * 切换到下一个显示页面（循环切换）。
 * 由 KEY3 短按触发（设置页面除外）。
 */
void DisplayChange(void);

/*
 * 通过 USART 打印当前页面名称，用于调试。
 * 由 Proc1SecTask() 调用。
 */
void PrintState(void);

/*
 * 获取当前显示状态。
 * 供 ProcKeyOne.c 使用，以决定按键动作的分发。
 */
DisplayState GetDisplayState(void);

#endif
