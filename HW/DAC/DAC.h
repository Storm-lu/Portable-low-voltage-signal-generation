#ifndef _DAC_H_
#define _DAC_H_


#include "DataType.h"


/*
 * Waveform buffer descriptor.
 * Used by DAC module to configure DMA source.
 *
 * waveBufAddr: address of u16 array containing waveform samples (0~4095)
 * waveBufSize: number of samples in the array (typically 100)
 */
typedef struct
{
    u32 waveBufAddr;
    u32 waveBufSize;
} StructDACWave;


/*
 * Initialize the DAC system.
 * Configures PA4, DAC Channel 1, TIM4 trigger, and DMA2 Channel 3.
 * Default waveform is triangle (100 points, 100Hz).
 * Called once during InitHardware().
 */
void InitDAC(void);

/*
 * Output a DC voltage on PA4.
 * Disables DMA waveform output, sets DAC register directly.
 *
 * param dacValue: 12-bit DAC value (0~4095)
 *   0V -> 0, 1.0V -> 1241, 2.0V -> 2482, 3.3V -> 4095
 *
 * Called by: SignalOutput.c -> OutputChange(), SetSignalOutputType()
 */
void StartDAC(u16 dacValue);

/*
 * Start DMA-driven waveform output on PA4.
 * Uses the waveform buffer set by SetDACWaveTri/SetDACWaveSine.
 * Waveform loops continuously via DMA circular mode.
 *
 * Called by: SignalOutput.c -> OutputChange(), SetSignalOutputType()
 */
void StartDACWave(void);

/*
 * Stop all DAC output (DC and waveform).
 * Disables DMA, sets DAC to 0V.
 *
 * Called by: SignalOutput.c, Main.c
 */
void StopDAC(void);

/*
 * Set the current waveform buffer descriptor.
 *
 * param wave: struct containing buffer address and size
 * Called by: SetDACWaveTri/SetDACWaveSine (internal), ProcHostCmd.c (external)
 */
void SetDACWave(StructDACWave wave);

/*
 * Switch waveform to triangle wave (100 points).
 * Updates internal buffer to point to triangle wave data.
 *
 * Called by: SignalOutput.c -> SetSignalOutputType()
 */
void SetDACWaveTri(void);

/*
 * Switch waveform to sine wave (100 points).
 * Updates internal buffer to point to sine wave data.
 *
 * Called by: SignalOutput.c -> SetSignalOutputType()
 */
void SetDACWaveSine(void);

#endif
