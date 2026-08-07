#ifndef _CAPTURE_H_
#define _CAPTURE_H_


#include "DataType.h"


/*
 * 输入捕获模块，用于频率和占空比测量。
 * 使用 TIM1 通道 1，引脚为 PA8。
 *
 * 硬件：PA8（输入），TIM1 CH1
 * 计数速率：72MHz / 72 = 1MHz（1 个计数 = 1 微秒）
 * 定时器周期：65535 个计数 = 65.535ms（支持最低约 15Hz 的频率）
 *
 * 测量方法：
 *   - 交替捕获上升沿和下降沿
 *   - 上升沿到上升沿的时间 = 周期 -> 频率
 *   - 上升沿到下降沿的时间 = 高电平时间 -> 占空比
 *   - 多周期平均（8 个样本），以获得稳定的读数
 *
 * 信号要求：
 *   - 数字信号（0V/3.3V 逻辑电平）
 *   - 不得超过 3.3V 或反向输入
 */


/*
 * 初始化输入捕获系统。
 * 配置 PA8、TIM1 CH1 和 NVIC 中断。
 * 在 InitHardware() 中调用一次。
 */
void InitCapture(void);

/*
 * 返回当前测得的频率（Hz）。
 *
 * 返回值：频率（Hz），若检测到无信号则返回 0
 *
 * 调用者：SignalMeasure.c -> DisplaySignalMeasure()、
 *         Main.c -> Proc1SecTask()
 */
u32 GetCaptureFreq(void);

/*
 * 返回当前测得的占空比。
 *
 * 返回值：0~100（百分比），若无信号/无效则返回 0xFF
 *
 * 调用者：SignalMeasure.c -> DisplaySignalMeasure()、
 *         Main.c -> Proc1SecTask()
 */
u8 GetCaptureDuty(void);

/*
 * 周期性无信号超时检测。
 * 必须由 Proc2msTask() 每 2ms 调用一次。
 *
 * 若 1 秒内无捕获事件（500 次调用）：
 *   - 将频率设为 0
 *   - 将占空比设为 0xFF（无效）
 *
 * 调用者：Main.c -> Proc2msTask()
 */
void CaptureTimeoutCheck(void);

#endif
