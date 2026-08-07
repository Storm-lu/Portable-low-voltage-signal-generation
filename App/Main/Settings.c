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
 * Initialize settings from Flash.
 *
 * Steps:
 *   1. Read 4 words from FLASH_SETTINGS_ADDR via STM32FlashReadWord
 *   2. Check if buf[0] == FLASH_MAGIC_NUMBER (valid data exists)
 *      If yes: load signal_mode, pwm_freq, pwm_duty from buf[1..3]
 *              Validate each enum value (guard against Flash corruption):
 *                signal_mode > SINE -> reset to DC_1V
 *                pwm_freq > PWM_FREQ_5KHz -> reset to PWM_FREQ_100Hz
 *                pwm_duty > PWM_DUTY_75 -> reset to PWM_DUTY_25
 *      If no:  use defaults (DC_1V, PWM_FREQ_1KHz, PWM_DUTY_50)
 *   3. Set current_setting = SETTING_SIGNAL_OUTPUT
 *
 * Must be called BEFORE InitSignalOutput() (which reads GetSettingsSignalMode).
 */
void InitSettings(void){
    /* TODO: Read from Flash, validate, load or use defaults. */
}

/*
 * Display the Settings page on OLED.
 *
 * Layout:
 *   y=0:  "Settings" (title)
 *   y=16: ">" or " " + "Mode:" + mode name (DC 1V / DC 2V / TRI / PWM / SINE)
 *
 *   The following lines only appear when signal_mode == PWM:
 *   y=32: ">" or " " + "Freq:" + frequency (100Hz / 1kHz / 5kHz)
 *   y=48: ">" or " " + "Duty:" + duty (25% / 50% / 75%)
 *
 * The ">" cursor indicates current_setting (which item KEY2 will modify).
 * Non-PWM modes: only Mode line shown, cursor locked to Mode.
 *
 * Remember: OLEDClear() at start, OLEDRefreshGRAM() at end.
 */
void DisplaySettings(void){
    /* TODO: Implement layout as described above. */
}

/*
 * Move cursor to next setting item (KEY1 action in Settings page).
 *
 * If signal_mode == PWM:
 *   Cycle: SETTING_SIGNAL_OUTPUT -> SETTING_PWM_FREQ -> SETTING_PWM_DUTY -> back
 * Else:
 *   Lock cursor at SETTING_SIGNAL_OUTPUT (only Mode is selectable)
 */
void ParamChange(void){
    /* TODO: Implement cursor cycling, gated by PWM mode. */
}

/*
 * Increment current setting value (KEY2 action in Settings page).
 *
 * SETTING_SIGNAL_OUTPUT: cycle signal_mode (DC_1V->DC_2V->TRI->PWM->SINE->DC_1V)
 *   If new mode != PWM, reset current_setting to SETTING_SIGNAL_OUTPUT
 *   (because Freq/Duty items disappear in non-PWM mode)
 * SETTING_PWM_FREQ: cycle pwm_freq (100Hz->1kHz->5kHz->100Hz)
 * SETTING_PWM_DUTY: cycle pwm_duty (25%->50%->75%->25%)
 */
void ValueChange(void){
    /* TODO: Implement value cycling for each setting item. */
}

/*
 * Save settings and exit (KEY3 long-press 2s action in Settings page).
 *
 * Steps:
 *   1. Apply signal output: SetSignalOutputType(signal_mode)
 *   2. If PWM mode: SetPWMFreq(pwm_freq), SetPWMDuty(pwm_duty)
 *   3. Write to Flash:
 *        buf[0] = FLASH_MAGIC_NUMBER
 *        buf[1] = (u32)signal_mode
 *        buf[2] = (u32)pwm_freq
 *        buf[3] = (u32)pwm_duty
 *        STM32FlashWriteWord(FLASH_SETTINGS_ADDR, buf, 4)
 *   4. Return to previous page: DisplayChange()
 */
void SaveSettings(void){
    /* TODO: Apply settings, write Flash, switch page. */
}

/* ============================================================
 *  Getter functions - already implemented, do NOT modify.
 *  These are called by SignalOutput.c to read saved parameters.
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
