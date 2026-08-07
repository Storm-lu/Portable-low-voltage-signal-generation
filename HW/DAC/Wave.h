#ifndef _WAVE_H_
#define _WAVE_H_


#include "DataType.h"


/*
 * Waveform data generator module.
 * Provides 100-point lookup tables for DAC waveform output.
 * All waveforms: 100 samples, u16 values, range 0~4095 (12-bit DAC).
 *
 * Waveform output frequency = DAC_sample_rate / 100
 *   e.g. 10kHz sample rate -> 100Hz output frequency
 */


/*
 * Initialize waveform data.
 * Can be empty if using compile-time lookup tables.
 */
void InitWave(void);

/*
 * Return pointer to 100-point sine wave data array.
 * Values: 0~4095, centered at 2048.
 *
 * Called by: DAC.c -> InitDAC(), SetDACWaveSine()
 */
u16* GetSineWave100PointAddr(void);

/*
 * Return pointer to 100-point rectangle (square) wave data array.
 * Values: 0 or 4095 (50% duty).
 *
 * Called by: DAC.c -> InitDAC(), ProcHostCmd.c -> OnGenWave()
 */
u16* GetRectWave100PointAddr(void);

/*
 * Return pointer to 100-point triangle wave data array.
 * Values: 0~4095, linear ramp up then down.
 *
 * Called by: DAC.c -> InitDAC(), SetDACWaveTri()
 */
u16* GetTriWave100PointAddr(void);

#endif
