#include "Wave.h"
#include "DataType.h"


/*
 * TODO: Generate 100-point waveform lookup tables.
 *
 * All waveforms: 100 samples per cycle, u16 values, range 0~4095 (12-bit DAC).
 *
 * Triangle wave (100 points):
 *   - Linear ramp from 0 to 4095 over first 50 points
 *   - Linear ramp from 4095 back to 0 over last 50 points
 *   - Step size: 4095 / 50 = 81.9 -> use 82 (approximate)
 *   - Formula: value[i] = (i < 50) ? i*82 : (99-i)*82
 *
 * Sine wave (100 points):
 *   - Centered at 2048 (midpoint of 0~4095)
 *   - Amplitude: 2047
 *   - Formula: value[i] = 2048 + 2047 * sin(2*PI*i / 100)
 *   - Cortex-M3 has no FPU, use integer approximation or precompute
 *
 * Rectangle wave (100 points):
 *   - First 50 points: 0
 *   - Last 50 points: 4095
 */


/* TODO: declare static u16 arrays for each waveform (100 points each) */


void InitWave(void)
{
    /* TODO: initialize waveform data (if using runtime calculation) */
}


/*
 * Return pointer to the 100-point sine wave data array.
 * The array must contain u16 values in range 0~4095.
 * DAC module uses this address to configure DMA source.
 */
u16* GetSineWave100PointAddr(void)
{
    /* TODO: return sine wave array address */
    return (u16*)0;
}


/*
 * Return pointer to the 100-point rectangle (square) wave data array.
 */
u16* GetRectWave100PointAddr(void)
{
    /* TODO: return rectangle wave array address */
    return (u16*)0;
}


/*
 * Return pointer to the 100-point triangle wave data array.
 */
u16* GetTriWave100PointAddr(void)
{
    /* TODO: return triangle wave array address */
    return (u16*)0;
}
