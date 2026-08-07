#ifndef _CAPTURE_H_
#define _CAPTURE_H_


#include "DataType.h"


/*
 * Input capture module for frequency and duty cycle measurement.
 * Uses TIM1 Channel 1 on PA8.
 *
 * Hardware: PA8 (input), TIM1 CH1
 * Counting rate: 72MHz / 72 = 1MHz (1 count = 1 microsecond)
 * Timer period: 65535 counts = 65.535ms (supports frequencies down to ~15Hz)
 *
 * Measurement method:
 *   - Alternate between rising and falling edge capture
 *   - Rising-to-rising edge time = period -> frequency
 *   - Rising-to-falling edge time = high time -> duty cycle
 *   - Multi-period averaging (8 samples) for stable readings
 *
 * Signal requirements:
 *   - Digital signal (0V/3.3V logic level)
 *   - Do NOT exceed 3.3V or go negative
 */


/*
 * Initialize input capture system.
 * Configures PA8, TIM1 CH1, and NVIC interrupts.
 * Called once during InitHardware().
 */
void InitCapture(void);

/*
 * Return the current measured frequency in Hz.
 *
 * Return: frequency in Hz, or 0 if no signal detected
 *
 * Called by: SignalMeasure.c -> DisplaySignalMeasure(),
 *            Main.c -> Proc1SecTask()
 */
u32 GetCaptureFreq(void);

/*
 * Return the current measured duty cycle.
 *
 * Return: 0~100 (percentage), or 0xFF if no signal / invalid
 *
 * Called by: SignalMeasure.c -> DisplaySignalMeasure(),
 *            Main.c -> Proc1SecTask()
 */
u8 GetCaptureDuty(void);

/*
 * Periodic no-signal timeout check.
 * Must be called every 2ms from Proc2msTask().
 *
 * If no capture event for 1 second (500 calls):
 *   - Sets frequency to 0
 *   - Sets duty cycle to 0xFF (invalid)
 *
 * Called by: Main.c -> Proc2msTask()
 */
void CaptureTimeoutCheck(void);

#endif
