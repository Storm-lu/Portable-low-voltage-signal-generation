#include "ADC.h"
#include "stm32f10x_conf.h"

/*
 * DMA circular buffer for ADC samples.
 * DMA1 Channel 1 writes ADC conversion results here continuously.
 * Size: ADC_BUF_SIZE (128) samples.
 */
static u16 s_arrADCSamples[ADC_BUF_SIZE];


/* ---- Internal function prototypes ---- */
static void ConfigADC1(void);
static void ConfigDMA1Ch1(void);


/*
 * Configure ADC1 Channel 1 on PA1.
 *
 * Required settings:
 *   - ADC clock: RCC_PCLK2_Div6 -> 72MHz/6 = 12MHz
 *   - PA1: GPIO_Mode_AIN (analog input)
 *   - ADC mode: Independent (single channel)
 *   - Scan mode: Disable (single channel)
 *   - Continuous conversion: Enable (auto-restart after each conversion)
 *   - External trigger: None (software start)
 *   - Data align: Right (12-bit in lower bits)
 *   - Channel: ADC_Channel_1, rank 1, sample time 239.5 cycles
 *   - Enable ADC DMA
 *   - Enable ADC
 *   - Run calibration (reset + wait + start + wait)
 *   - Start software conversion
 *
 * ADC sample rate = ADC_clock / (sample_time + conversion_time)
 *   = 12MHz / (239.5 + 12.5) = 12MHz / 252 = ~47.6kHz
 */
static void ConfigADC1(void)
{
    /* TODO: implement ADC1 + PA1 configuration */
}


/*
 * Configure DMA1 Channel 1 to transfer ADC results to memory.
 *
 * Required settings:
 *   - Direction: Peripheral -> Memory
 *   - Peripheral address: &ADC1->DR (ADC data register)
 *   - Memory address: s_arrADCSamples
 *   - Buffer size: ADC_BUF_SIZE (128)
 *   - Mode: Circular (continuous capture)
 *   - Data size: HalfWord (16-bit) for both
 *   - Peripheral increment: Disable, Memory increment: Enable
 *   - Priority: Medium
 */
static void ConfigDMA1Ch1(void)
{
    /* TODO: implement DMA1 Channel 1 configuration */
}


/*
 * Initialize ADC system.
 * Call ConfigADC1() then ConfigDMA1Ch1().
 */
void InitADC(void)
{
    /* TODO: initialize ADC system */
}


/*
 * Return pointer to the ADC sample buffer (128 samples, u16).
 * The buffer is continuously updated by DMA in circular mode.
 *
 * Called by: SignalMeasure.c -> DisplaySignalMeasure()
 */
u16* GetADCBuf(void)
{
    /* TODO: return s_arrADCSamples pointer */
    return (u16*)0;
}


/*
 * Return the most recent ADC sample value.
 * Read DMA1 Channel 1 current data counter to find the latest written index.
 *
 * Called by: (optional, for single-sample reads)
 */
u16 GetADCLatest(void)
{
    /* TODO: read DMA counter, calculate latest index, return sample */
    return 0;
}


/*
 * Compute statistics over the entire ADC buffer.
 *
 * Parameters are output pointers:
 *   pAvg: average of all 128 samples (0~4095)
 *   pMax: maximum value found (0~4095)
 *   pMin: minimum value found (0~4095)
 *   pP2P: peak-to-peak = max - min (0~4095)
 *
 * Voltage conversion: voltage_mV = value * 3300 / 4095
 *
 * Called by: SignalMeasure.c, Main.c (Proc1SecTask)
 */
void GetADCStats(u16 *pAvg, u16 *pMax, u16 *pMin, u16 *pP2P)
{
    /* TODO: iterate s_arrADCSamples, compute avg/max/min/p2p */
}
