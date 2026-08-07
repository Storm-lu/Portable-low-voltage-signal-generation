#ifndef __SIGNALMEASURE__H
#define __SIGNALMEASURE__H

/*
 * Initialize signal measure module (currently empty).
 */
void InitSignalMeasure(void);

/*
 * Display the Signal Measure page on OLED.
 *
 * Shows:
 *   - Left 80x48 area: auto-scaled waveform with dotted grid background
 *   - Right 48px column: V (peak), F (frequency), P (peak-to-peak), D (duty)
 *
 * Calls Team A's GetADCBuf/GetADCStats and Team B's GetCaptureFreq/GetCaptureDuty.
 * Called from DisplayProcess() at 10Hz.
 */
void DisplaySignalMeasure(void);

#endif
