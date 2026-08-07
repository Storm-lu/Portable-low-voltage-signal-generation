#ifndef __SETTINGS_H
#define __SETTINGS_H

#include "SignalOutput.h"
#include "PWM.h"

/*
 * 设置页面中可选的设置项。
 * 光标（current_setting）通过 KEY1 在这些项之间循环。
 */
typedef enum{
    SETTING_SIGNAL_OUTPUT,   /* 输出模式：DC_1V/DC_2V/TRI/PWM/SINE */
    SETTING_PWM_FREQ,        /* PWM 频率：仅 mode==PWM 时显示 */
    SETTING_PWM_DUTY         /* PWM 占空比：仅 mode==PWM 时显示 */
} SettingItem;

/*
 * 从 Flash 初始化设置。
 * 读取 0x0803F800 处的 4 个字，校验魔数和枚举范围。
 * 必须在 InitSignalOutput() 之前调用。
 */
void InitSettings(void);

/*
 * 在 OLED 上显示设置页面。
 * 始终显示 Mode；Freq/Duty 仅在 mode==PWM 时显示。
 */
void DisplaySettings(void);

/*
 * 移动光标到下一个设置项（设置页面的 KEY1）。
 * 非 PWM 模式：光标停留在 Mode。
 */
void ParamChange(void);

/*
 * 递增当前设置值（设置页面的 KEY2）。
 * 循环切换。从 PWM 切换到其他模式时光标复位到 Mode。
 */
void ValueChange(void);

/*
 * 将设置保存到 Flash 并应用到输出（KEY3 长按 2秒）。
 * 调用 SetSignalOutputType、SetPWMFreq/SetPWMDuty，写 Flash，退出页面。
 */
void SaveSettings(void);

/*
 * 获取已保存的信号输出模式（开机时从 Flash 读取）。
 * 由 SignalOutput.c 的 InitSignalOutput() 调用。
 */
SignalOutputType GetSettingsSignalMode(void);

/*
 * 获取已保存的 PWM 频率。
 * 由 SignalOutput.c 启动 PWM 输出时调用。
 */
PWMFreq GetSettingsPWMFreq(void);

/*
 * 获取已保存的 PWM 占空比。
 * 由 SignalOutput.c 启动 PWM 输出时调用。
 */
PWMDuty GetSettingsPWMDuty(void);

#endif
