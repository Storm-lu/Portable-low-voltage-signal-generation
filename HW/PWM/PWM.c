#include "PWM.h"
#include "stm32f10x_conf.h"


/*
 * Frequency lookup table: ARR values for each PWMFreq option.
 * Counting rate = 72MHz / (PSC+1) = 72MHz / 72 = 1MHz
 * PWM frequency = 1MHz / (ARR+1)
 *
 * Index:  PWM_FREQ_100Hz -> 9999  -> 1MHz/10000  = 100Hz
 *         PWM_FREQ_1KHz  -> 999   -> 1MHz/1000   = 1kHz
 *         PWM_FREQ_5KHz  -> 199   -> 1MHz/200    = 5kHz
 */
static const u16 s_freqArr[] = {9999, 999, 199};

/*
 * Duty cycle lookup table: permille (0~1000) for each PWMDuty option.
 * CCR = (ARR+1) * duty_permille / 1000
 *
 * Index:  PWM_DUTY_25 -> 250  -> 25%
 *         PWM_DUTY_50 -> 500  -> 50%
 *         PWM_DUTY_75 -> 750  -> 75%
 */
static const u16 s_dutyArr[] = {250, 500, 750};

static PWMFreq s_freq = PWM_FREQ_1KHz;
static PWMDuty s_duty = PWM_DUTY_50;


/* ---- Internal function prototype ---- */
static void ConfigTimer3ForPWMPB5(u16 arr, u16 psc);


/*
 * Configure TIM3 Channel 2 for PWM output on PB5.
 *
 * Required settings:
 *   - Enable TIM3, GPIOB, AFIO clocks
 *   - GPIO_PartialRemap_TIM3: remap TIM3_CH2 from PA7 to PB5
 *   - PB5: GPIO_Mode_AF_PP (alternate function push-pull)
 *   - TIM3: up-counting, PSC=71 (1MHz counting rate)
 *   - TIM3 CH2: PWM mode 2, output enable, polarity Low
 *   - Enable TIM3 preload for OC2
 *
 * param arr: auto-reload value (determines frequency)
 * param psc: prescaler (typically 71 for 1MHz counting)
 */
static void ConfigTimer3ForPWMPB5(u16 arr, u16 psc)
{
    /* TODO: implement TIM3 CH2 PWM configuration on PB5 */
}


/*
 * Initialize PWM with default settings (1kHz, 50% duty).
 * Configure timer and set initial CCR to 0 (no output).
 */
void InitPWM(void)
{
    /* TODO: initialize PWM system */
}


/*
 * Start PWM output with specified frequency and duty cycle.
 *
 * Steps:
 *   1. Save freq and duty to internal state
 *   2. Configure TIM3 with s_freqArr[freq] and PSC=71
 *   3. Calculate CCR = (ARR+1) * s_dutyArr[duty] / 1000
 *   4. Set CCR via TIM_SetCompare2
 *   5. Enable TIM3
 *
 * param freq: PWMFreq enum (100Hz / 1kHz / 5kHz)
 * param duty: PWMDuty enum (25% / 50% / 75%)
 *
 * Called by: SignalOutput.c -> OutputChange(), SetSignalOutputType()
 */
void StartPWM(PWMFreq freq, PWMDuty duty)
{
    /* TODO: start PWM with given parameters */
}


/*
 * Stop PWM output.
 * Set CCR to 0 and disable TIM3.
 *
 * Called by: SignalOutput.c, Main.c
 */
void StopPWM(void)
{
    /* TODO: stop PWM output */
}


/*
 * Change PWM frequency without restarting timer.
 * Use TIM_SetAutoreload to change ARR, then recalculate CCR for current duty.
 *
 * param freq: new PWMFreq enum value
 *
 * Called by: Settings.c -> SaveSettings()
 */
void SetPWMFreq(PWMFreq freq)
{
    /* TODO: change PWM frequency on the fly */
}


/*
 * Change PWM duty cycle without restarting timer.
 * Recalculate CCR = (ARR+1) * s_dutyArr[duty] / 1000 using current ARR.
 *
 * param duty: new PWMDuty enum value
 *
 * Called by: Settings.c -> SaveSettings()
 */
void SetPWMDuty(PWMDuty duty)
{
    /* TODO: change PWM duty cycle on the fly */
}
