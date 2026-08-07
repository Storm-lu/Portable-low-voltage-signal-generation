#ifndef __SETTINGS_H
#define __SETTINGS_H

#include "SignalOutput.h"
#include "PWM.h"

/*
 * Setting items selectable in Settings page.
 * The cursor (current_setting) cycles through these with KEY1.
 */
typedef enum{
    SETTING_SIGNAL_OUTPUT,   /* Output mode: DC_1V/DC_2V/TRI/PWM/SINE */
    SETTING_PWM_FREQ,        /* PWM frequency: only shown when mode==PWM */
    SETTING_PWM_DUTY         /* PWM duty: only shown when mode==PWM */
} SettingItem;

/*
 * Initialize settings from Flash.
 * Reads 4 words at 0x0803F800, validates magic number and enum ranges.
 * Must be called before InitSignalOutput().
 */
void InitSettings(void);

/*
 * Display the Settings page on OLED.
 * Shows Mode always; Freq/Duty only when mode==PWM.
 */
void DisplaySettings(void);

/*
 * Move cursor to next setting item (KEY1 in Settings page).
 * Non-PWM mode: cursor stays on Mode.
 */
void ParamChange(void);

/*
 * Increment current setting value (KEY2 in Settings page).
 * Cycling wraps around. Switching away from PWM resets cursor to Mode.
 */
void ValueChange(void);

/*
 * Save settings to Flash and apply to output (KEY3 long-press 2s).
 * Calls SetSignalOutputType, SetPWMFreq/SetPWMDuty, writes Flash, exits page.
 */
void SaveSettings(void);

/*
 * Get saved signal output mode (read from Flash at boot).
 * Called by SignalOutput.c InitSignalOutput().
 */
SignalOutputType GetSettingsSignalMode(void);

/*
 * Get saved PWM frequency.
 * Called by SignalOutput.c when starting PWM output.
 */
PWMFreq GetSettingsPWMFreq(void);

/*
 * Get saved PWM duty cycle.
 * Called by SignalOutput.c when starting PWM output.
 */
PWMDuty GetSettingsPWMDuty(void);

#endif
