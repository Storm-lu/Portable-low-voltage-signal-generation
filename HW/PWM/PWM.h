#ifndef _PWM_H_
#define _PWM_H_


#include "DataType.h"


/*
 * PWM 输出模块。
 * 使用 TIM3 通道 2，部分重映射 -> 在 PB5 上输出。
 *
 * 硬件：PB5（PWM 输出），TIM3 CH2
 * 计数速率：72MHz / 72 = 1MHz
 * PWM 频率 = 1MHz / (ARR+1)
 *
 * 可选频率：100Hz、1kHz、5kHz
 * 可选占空比：25%、50%、75%
 * 输出电压：0~3.3V
 */


/*
 * PWM 频率选项。
 * 对应 ARR 值：{9999, 999, 199} -> {100Hz, 1kHz, 5kHz}
 */
typedef enum{
    PWM_FREQ_100Hz,
    PWM_FREQ_1KHz,
    PWM_FREQ_5KHz
} PWMFreq;

/*
 * PWM 占空比选项。
 * 对应千分比：{250, 500, 750} -> {25%, 50%, 75%}
 */
typedef enum{
    PWM_DUTY_25,
    PWM_DUTY_50,
    PWM_DUTY_75
} PWMDuty;


/*
 * 使用默认设置（1kHz，50% 占空比）初始化 PWM 系统。
 * 配置 TIM3 CH2 在 PB5 上，但不启动输出（CCR=0）。
 * 在 InitHardware() 中调用一次。
 */
void InitPWM(void);

/*
 * 以指定的频率和占空比启动 PWM 输出。
 * 配置定时器并使能输出。
 *
 * 参数 freq：PWMFreq 枚举值（100Hz / 1kHz / 5kHz）
 * 参数 duty：PWMDuty 枚举值（25% / 50% / 75%）
 *
 * 调用者：SignalOutput.c -> OutputChange()、SetSignalOutputType()
 */
void StartPWM(PWMFreq freq, PWMDuty duty);

/*
 * 停止 PWM 输出。将 CCR 设为 0，禁用定时器。
 *
 * 调用者：SignalOutput.c、Main.c
 */
void StopPWM(void);

/*
 * 运行中修改 PWM 频率（不重启定时器）。
 * 通过 TIM_SetAutoreload 更新 ARR，根据当前占空比重算 CCR。
 *
 * 参数 freq：新的 PWMFreq 枚举值
 *
 * 调用者：Settings.c -> SaveSettings()
 */
void SetPWMFreq(PWMFreq freq);

/*
 * 运行中修改 PWM 占空比。
 * 根据当前 ARR 和新占空比重算 CCR。
 *
 * 参数 duty：新的 PWMDuty 枚举值
 *
 * 调用者：Settings.c -> SaveSettings()
 */
void SetPWMDuty(PWMDuty duty);

#endif
