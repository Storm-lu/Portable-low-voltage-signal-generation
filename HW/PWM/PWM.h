#ifndef _PWM_H_
#define _PWM_H_


#include "DataType.h"


/*
 * PWM output module.
 * Uses TIM3 Channel 2 with partial remap -> output on PB5.
 *
 * Hardware: PB5 (PWM output), TIM3 CH2
 * Counting rate: 72MHz / 72 = 1MHz
 * PWM frequency = 1MHz / (ARR+1)
 *
 * Available frequencies: 100Hz, 1kHz, 5kHz
 * Available duty cycles: 25%, 50%, 75%
 * Output voltage: 0~3.3V
 */


/*
 * PWM frequency options.
 * Maps to ARR values: {9999, 999, 199} -> {100Hz, 1kHz, 5kHz}
 */
typedef enum{
    PWM_FREQ_100Hz,
    PWM_FREQ_1KHz,
    PWM_FREQ_5KHz
} PWMFreq;

/*
 * PWM duty cycle options.
 * Maps to permille values: {250, 500, 750} -> {25%, 50%, 75%}
 */
typedef enum{
    PWM_DUTY_25,
    PWM_DUTY_50,
    PWM_DUTY_75
} PWMDuty;


/*
 * Initialize PWM system with default settings (1kHz, 50% duty).
 * Configures TIM3 CH2 on PB5, but does not start output (CCR=0).
 * Called once during InitHardware().
 */
void InitPWM(void);

/*
 * Start PWM output with specified frequency and duty cycle.
 * Configures timer and enables output.
 *
 * param freq: PWMFreq enum (100Hz / 1kHz / 5kHz)
 * param duty: PWMDuty enum (25% / 50% / 75%)
 *
 * Called by: SignalOutput.c -> OutputChange(), SetSignalOutputType()
 */
void StartPWM(PWMFreq freq, PWMDuty duty);

/*
 * Stop PWM output. Sets CCR to 0, disables timer.
 *
 * Called by: SignalOutput.c, Main.c
 */
void StopPWM(void);

/*
 * Change PWM frequency on the fly (without restarting timer).
 * Updates ARR via TIM_SetAutoreload, recalculates CCR for current duty.
 *
 * param freq: new PWMFreq enum value
 *
 * Called by: Settings.c -> SaveSettings()
 */
void SetPWMFreq(PWMFreq freq);

/*
 * Change PWM duty cycle on the fly.
 * Recalculates CCR based on current ARR and new duty.
 *
 * param duty: new PWMDuty enum value
 *
 * Called by: Settings.c -> SaveSettings()
 */
void SetPWMDuty(PWMDuty duty);

#endif
