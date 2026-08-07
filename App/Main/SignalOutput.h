#ifndef __SIGNALOUTPUT__H
#define __SIGNALOUTPUT__H

/*
 * Signal output modes.
 * Order matters: DC_1V=0, DC_2V=1, TRIANGLE=2, PWM=3, SINE=4
 * Settings page cycles through these with KEY2.
 */
typedef enum{
    DC_1V,
    DC_2V,
    TRIANGLE,
    PWM,
    SINE
} SignalOutputType;

/*
 * Initialize signal output module.
 * Reads saved mode from Settings (Flash), sets output to stopped.
 */
void InitSignalOutput(void);

/*
 * Display the Signal Output page on OLED.
 * Shows: mode name, running/stopped status, key hint.
 */
void DisplaySignalOutput(void);

/*
 * Toggle output start/stop (KEY1 action on Signal Output page).
 * Starts DAC/PWM based on current mode, or stops everything.
 */
void OutputChange(void);

/*
 * Switch signal output type and restart output immediately.
 * Called from SaveSettings() when user changes mode in Settings page.
 *
 * For TRIANGLE/SINE: calls SetDACWaveTri/Sine + StartDACWave.
 * For PWM: calls StartPWM with Settings parameters.
 * For DC: calls StartDAC with calibrated voltage values.
 */
void SetSignalOutputType(SignalOutputType type);

/*
 * Get current signal output type.
 * Used by Proc1SecTask (USART output) and Settings.c.
 */
SignalOutputType GetSignalOutputType(void);

#endif
