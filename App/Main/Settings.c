#include "Main.h"
#include "stm32f10x_conf.h"
#include "DataType.h"
#include "Settings.h"
#include "PWM.h"
#include "SignalOutput.h"
#include "Flash.h"
#include "OLED.h"

#define FLASH_SETTINGS_ADDR  0x0803F800UL
#define FLASH_MAGIC_NUMBER   0x12345678UL

static SignalOutputType signal_mode = DC_1V;
static PWMFreq pwm_freq = PWM_FREQ_1KHz;
static PWMDuty pwm_duty = PWM_DUTY_50;
static SettingItem current_setting = SETTING_SIGNAL_OUTPUT;

/*
 * 从 Flash 初始化设置。
 *
 * 步骤：
 *   1. 通过 STM32FlashReadWord 从 FLASH_SETTINGS_ADDR 读取 4 个字
 *   2. 检查 buf[0] == FLASH_MAGIC_NUMBER（存在有效数据）
 *      若是：从 buf[1..3] 加载 signal_mode、pwm_freq、pwm_duty
 *            校验每个枚举值（防止 Flash 损坏）：
 *              signal_mode > SINE -> 复位为 DC_1V
 *              pwm_freq > PWM_FREQ_5KHz -> 复位为 PWM_FREQ_100Hz
 *              pwm_duty > PWM_DUTY_75 -> 复位为 PWM_DUTY_25
 *      若否：使用默认值（DC_1V, PWM_FREQ_1KHz, PWM_DUTY_50）
 *   3. 设 current_setting = SETTING_SIGNAL_OUTPUT
 *
 * 必须在 InitSignalOutput() 之前调用（后者会读取 GetSettingsSignalMode）。
 */
void InitSettings(void){
    /* 待实现：从 Flash 读取、校验、加载或使用默认值。 */
}

/*
 * 在 OLED 上显示设置页面。
 *
 * 布局：
 *   y=0：  "Settings"（标题）
 *   y=16： ">" 或 " " + "Mode:" + 模式名称（DC 1V / DC 2V / TRI / PWM / SINE）
 *
 *   以下两行仅在 signal_mode == PWM 时显示：
 *   y=32： ">" 或 " " + "Freq:" + 频率（100Hz / 1kHz / 5kHz）
 *   y=48： ">" 或 " " + "Duty:" + 占空比（25% / 50% / 75%）
 *
 * ">" 光标指示 current_setting（即 KEY2 将修改的项）。
 * 非 PWM 模式：只显示 Mode 行，光标锁定在 Mode。
 *
 * 注意：开头调用 OLEDClear()，结尾调用 OLEDRefreshGRAM()。
 */
void DisplaySettings(void){
    /* 待实现：按上述说明实现布局。 */
}

/*
 * 移动光标到下一个设置项（设置页面的 KEY1 动作）。
 *
 * 若 signal_mode == PWM：
 *   循环：SETTING_SIGNAL_OUTPUT -> SETTING_PWM_FREQ -> SETTING_PWM_DUTY -> 回到起点
 * 否则：
 *   光标锁定在 SETTING_SIGNAL_OUTPUT（仅 Mode 可选）
 */
void ParamChange(void){
    /* 待实现：实现光标循环，受 PWM 模式限制。 */
}

/*
 * 递增当前设置值（设置页面的 KEY2 动作）。
 *
 * SETTING_SIGNAL_OUTPUT：循环 signal_mode（DC_1V->DC_2V->TRI->PWM->SINE->DC_1V）
 *   若新模式 != PWM，将 current_setting 复位为 SETTING_SIGNAL_OUTPUT
 *   （因为非 PWM 模式下 Freq/Duty 项不可见）
 * SETTING_PWM_FREQ：循环 pwm_freq（100Hz->1kHz->5kHz->100Hz）
 * SETTING_PWM_DUTY：循环 pwm_duty（25%->50%->75%->25%）
 */
void ValueChange(void){
    /* 待实现：为每个设置项实现值循环。 */
}

/*
 * 保存设置并退出（设置页面的 KEY3 长按 2秒 动作）。
 *
 * 步骤：
 *   1. 应用信号输出：SetSignalOutputType(signal_mode)
 *   2. 若为 PWM 模式：SetPWMFreq(pwm_freq), SetPWMDuty(pwm_duty)
 *   3. 写入 Flash：
 *        buf[0] = FLASH_MAGIC_NUMBER
 *        buf[1] = (u32)signal_mode
 *        buf[2] = (u32)pwm_freq
 *        buf[3] = (u32)pwm_duty
 *        STM32FlashWriteWord(FLASH_SETTINGS_ADDR, buf, 4)
 *   4. 返回上一页面：DisplayChange()
 */
void SaveSettings(void){
    /* 待实现：应用设置、写 Flash、切换页面。 */
}

/* ============================================================
 *  Getter 函数 - 已实现，请勿修改。
 *  由 SignalOutput.c 调用以读取已保存的参数。
 * ============================================================ */

SignalOutputType GetSettingsSignalMode(void){
    return signal_mode;
}

PWMFreq GetSettingsPWMFreq(void){
    return pwm_freq;
}

PWMDuty GetSettingsPWMDuty(void){
    return pwm_duty;
}
