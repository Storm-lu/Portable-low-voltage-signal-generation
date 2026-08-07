#ifndef _ADC_H_
#define _ADC_H_


#include "DataType.h"


/*
 * ADC signal measurement module.
 * Uses ADC1 Channel 1 (PA1) + DMA1 Channel 1 for continuous sampling.
 *
 * Hardware: PA1 (analog input), ADC1, DMA1 Channel 1
 * ADC clock: 12MHz, sample time: 239.5 cycles -> ~47.6kHz sample rate
 * Buffer: 128 samples in circular DMA mode
 *
 * Voltage range: 0~3.3V -> ADC value 0~4095 (12-bit)
 * WARNING: Input must not go negative or exceed 3.3V (damages chip)
 */


/* Number of samples in the DMA circular buffer */
#define ADC_BUF_SIZE 128

/*
 * Initialize ADC1 + DMA1 Channel 1.
 * Configures PA1 as analog input, starts continuous conversion + DMA.
 * Called once during InitHardware().
 */
void InitADC(void);

/*
 * Return pointer to the 128-sample ADC buffer (u16 array).
 * Buffer is continuously updated by DMA in circular mode.
 * Note: data may be partially updated while reading (DMA is active).
 *
 * Return: pointer to s_arrADCSamples[ADC_BUF_SIZE]
 *
 * Called by: SignalMeasure.c -> DisplaySignalMeasure()
 */
u16* GetADCBuf(void);

/*
 * Return the most recent single ADC sample.
 * Uses DMA remaining counter to find the latest written position.
 *
 * Return: u16 ADC value (0~4095)
 *
 * Called by: (optional, for single-sample polling)
 */
u16 GetADCLatest(void);

/*
 * Compute statistics over the full ADC buffer (128 samples).
 *
 * param pAvg: output average value (0~4095)
 * param pMax: output maximum value (0~4095)
 * param pMin: output minimum value (0~4095)
 * param pP2P: output peak-to-peak = max - min (0~4095)
 *
 * Voltage conversion: mV = value * 3300 / 4095
 *
 * Called by: SignalMeasure.c, Main.c -> Proc1SecTask()
 */
void GetADCStats(u16 *pAvg, u16 *pMax, u16 *pMin, u16 *pP2P);

#endif
