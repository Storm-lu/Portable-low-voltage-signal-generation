#include "Capture.h"
#include "stm32f10x_conf.h"


/*
 * Frequency measurement result (Hz). 0 = no signal.
 * Updated by TIM1_CC_IRQHandler, read by GetCaptureFreq().
 */
static volatile u32 s_freq      = 0;

/*
 * Duty cycle measurement (0~100). 0xFF = invalid/no signal.
 * Updated by TIM1_CC_IRQHandler, read by GetCaptureDuty().
 */
static volatile u8  s_duty      = 0xFF;

/*
 * Edge tracking: 0 = waiting for rising edge, 1 = waiting for falling edge.
 * Alternates between rising and falling edges to measure period and duty.
 */
static volatile u8  s_edgeMode  = 0;

/*
 * TIM1 overflow counter. Each overflow = 65536 counts = 65.536ms at 1MHz.
 * Used to measure periods longer than one timer cycle.
 */
static volatile u16 s_ovfCnt    = 0;

/*
 * Last captured CCR value (timer count at last edge).
 * Used to calculate time difference between edges.
 */
static volatile u16 s_lastCCR   = 0;

/*
 * Last measured complete period (in timer counts = microseconds at 1MHz).
 * Saved on rising edge, used by falling edge to compute duty cycle.
 */
static volatile u32 s_lastPeriod = 0;

/*
 * No-signal counter, incremented every 2ms by CaptureTimeoutCheck().
 * If it reaches 500 (1 second), frequency/duty are reset to "no signal".
 */
static volatile u16 s_noSignalCnt = 0;


/*
 * Multi-period averaging buffer (advanced feature).
 * Stores recent frequency measurements for averaging.
 */
#define AVG_COUNT 8
static volatile u32 s_freqBuf[AVG_COUNT];
static volatile u8  s_freqIdx = 0;
static volatile u8  s_freqValid = 0;


/*
 * Configure TIM1 Channel 1 input capture on PA8.
 *
 * Required settings:
 *   - PA8: GPIO_Mode_IPU (input pull-up, for digital signal input)
 *   - TIM1: PSC=71 (72MHz/72 = 1MHz, 1 count = 1us), ARR=65535
 *   - TIM1 CH1: input capture, rising edge, direct TI, no prescaler, no filter
 *   - Enable TIM_IT_CC1 (capture interrupt) and TIM_IT_Update (overflow interrupt)
 *   - NVIC: TIM1_CC_IRQn (priority 0/0), TIM1_UP_IRQn (priority 0/1)
 *   - Enable TIM1
 *
 * Measurement range: ~15Hz (65.5ms period) to ~1MHz (limited by interrupt latency)
 */
static void ConfigCapture(void)
{
    /* TODO: implement TIM1 CH1 input capture configuration on PA8 */
}


/*
 * Initialize the capture system.
 * Call ConfigCapture().
 */
void InitCapture(void)
{
    /* TODO: initialize input capture */
}


/*
 * Return the current measured frequency in Hz.
 * Return 0 if no signal detected (timeout exceeded).
 *
 * Called by: SignalMeasure.c, Main.c -> Proc1SecTask()
 */
u32 GetCaptureFreq(void)
{
    /* TODO: return s_freq */
    return 0;
}


/*
 * Return the current measured duty cycle (0~100).
 * Return 0xFF if no signal or measurement invalid.
 *
 * Called by: SignalMeasure.c, Main.c -> Proc1SecTask()
 */
u8 GetCaptureDuty(void)
{
    /* TODO: return s_duty */
    return 0xFF;
}


/*
 * Periodic timeout check, called every 2ms from Proc2msTask().
 *
 * If no capture event occurs for 1 second (500 x 2ms):
 *   - Set s_freq = 0 (no signal)
 *   - Set s_duty = 0xFF (invalid)
 *
 * The capture ISR resets s_noSignalCnt to 0 on each valid edge.
 */
void CaptureTimeoutCheck(void)
{
    /* TODO: implement no-signal timeout detection */
}


/*
 * TIM1 overflow interrupt handler.
 *
 * Called when TIM1 counter wraps from 65535 to 0.
 * Increment s_ovfCnt to track total elapsed time for long periods.
 *
 * Must clear TIM_IT_Update pending bit.
 */
void TIM1_UP_IRQHandler(void)
{
    /* TODO: handle timer overflow */
}


/*
 * TIM1 capture/compare interrupt handler.
 *
 * This is the core measurement algorithm:
 *
 * On RISING edge (s_edgeMode == 0):
 *   1. Calculate total count = s_ovfCnt * 65536 + curCCR - s_lastCCR
 *   2. Period (us) = total count (since 1MHz counting rate)
 *   3. Frequency = 1000000 / period
 *   4. Store in averaging buffer, compute average -> s_freq
 *   5. Save s_lastPeriod for the falling-edge calculation
 *   6. Save s_lastCCR = curCCR, reset s_ovfCnt = 0
 *   7. Switch to falling edge: TIM1->CCER |= TIM_CCER_CC1P
 *   8. Set s_edgeMode = 1
 *
 * On FALLING edge (s_edgeMode == 1):
 *   1. Calculate highTime = s_ovfCnt * 65536 + curCCR - s_lastCCR
 *   2. Duty = highTime * 100 / s_lastPeriod
 *   3. Clamp duty to 0~100, store in s_duty
 *   4. Switch back to rising edge: TIM1->CCER &= ~TIM_CCER_CC1P
 *   5. Set s_edgeMode = 0
 *
 * On ANY edge:
 *   - Clear TIM_IT_CC1 pending bit
 *   - Reset s_noSignalCnt = 0 (signal is present)
 *
 * NOTE: TIM_SetIC1Polarity does not exist in the stdlib.
 *       Use TIM1->CCER register directly:
 *         Rising edge:  TIM1->CCER &= ~TIM_CCER_CC1P
 *         Falling edge: TIM1->CCER |= TIM_CCER_CC1P
 */
void TIM1_CC_IRQHandler(void)
{
    /* TODO: implement capture interrupt handler */
}
