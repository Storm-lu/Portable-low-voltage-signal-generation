#ifndef __SIGNALMEASURE__H
#define __SIGNALMEASURE__H

/*
 * 初始化信号测量模块（目前为空）。
 */
void InitSignalMeasure(void);

/*
 * 在 OLED 上显示信号测量页面。
 *
 * 显示内容：
 *   - 左侧 80x48 区域：带点状网格背景的自动缩放波形
 *   - 右侧 48px 列：V（峰值）、F（频率）、P（峰峰值）、D（占空比）
 *
 * 调用 A组的 GetADCBuf/GetADCStats 和 B组的 GetCaptureFreq/GetCaptureDuty。
 * 由 DisplayProcess() 以 10Hz 频率调用。
 */
void DisplaySignalMeasure(void);

#endif
