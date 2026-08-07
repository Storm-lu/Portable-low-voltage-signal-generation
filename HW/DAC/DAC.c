#include "DAC.h"
#include "Wave.h"
#include "stm32f10x_conf.h"


/*
 * DAC1 12-bit right-aligned data holding register address.
 * DMA writes waveform sample data to this register.
 */
#define DAC_DHR12R1_ADDR    ((u32)0x40007408)


/*
 * Current waveform buffer descriptor.
 * Updated by SetDACWaveTri/SetDACWaveSine, read by StartDACWave.
 */
static StructDACWave s_strDAC1WaveBuf;


/* ---- Internal function prototypes ---- */
static void ConfigTimer4(u16 arr, u16 psc);
static void ConfigDAC1(void);
static void ConfigDMA2Ch3ForDAC1(StructDACWave wave);


/*
 * Configure TIM4 as the DAC trigger source.
 *
 * Clock: APB1 = 72MHz
 * Counting rate = 72MHz / (PSC+1)
 * Trigger frequency = counting_rate / (ARR+1) = DAC sample rate
 *
 * Required settings:
 *   - TIM_TRGOSource_Update: trigger DAC on each update event
 *   - Up-counting mode
 *
 * Example: arr=99, psc=71 -> 72MHz/72/100 = 10kHz sample rate
 *          With 100-point waveform -> 100Hz output frequency
 */
static void ConfigTimer4(u16 arr, u16 psc)
{
    /* TODO: implement TIM4 configuration */
}


/*
 * Configure DAC Channel 1 output on PA4.
 *
 * Required settings:
 *   - PA4: GPIO_Mode_AIN (analog input)
 *   - DAC trigger: DAC_Trigger_T4_TRGO (triggered by TIM4)
 *   - DAC output buffer: enabled
 *   - Initial data: 0
 *   - Enable DAC Channel 1
 */
static void ConfigDAC1(void)
{
    /* TODO: implement DAC1 + PA4 GPIO configuration */
}


/*
 * Configure DMA2 Channel 3 to feed waveform data to DAC1.
 *
 * Required settings:
 *   - Direction: Memory -> Peripheral
 *   - Peripheral address: DAC_DHR12R1_ADDR (0x40007408)
 *   - Memory address: wave.waveBufAddr
 *   - Buffer size: wave.waveBufSize
 *   - Mode: Circular (loop waveform endlessly)
 *   - Data size: HalfWord (16-bit) for both peripheral and memory
 *   - Peripheral increment: Disable, Memory increment: Enable
 *   - Priority: High
 */
static void ConfigDMA2Ch3ForDAC1(StructDACWave wave)
{
    /* TODO: implement DMA2 Channel 3 configuration */
}


/*
 * Initialize the DAC system.
 *
 * Steps:
 *   1. Set default waveform to triangle (100 points from Wave module)
 *   2. Call ConfigDAC1() to set up PA4 and DAC Channel 1
 *   3. Call ConfigTimer4(99, 71) for 10kHz sample rate (= 100Hz waveform)
 *
 * NOTE: Do NOT enable DAC DMA here. DMA is started later by StartDACWave().
 */
void InitDAC(void)
{
    /* TODO: initialize DAC system */
}


/*
 * Update the internal waveform buffer descriptor.
 * Called by SetDACWaveTri/SetDACWaveSine to switch waveform data.
 */
void SetDACWave(StructDACWave wave)
{
    /* TODO: update s_strDAC1WaveBuf */
}


/*
 * Output a DC voltage on PA4.
 *
 * Steps:
 *   1. Disable DAC DMA and DMA2 Channel 3 (stop any waveform output)
 *   2. Write dacValue directly to DAC data register
 *
 * dacValue: 12-bit value (0~4095)
 *   0V -> 0, 1.0V -> 1241, 2.0V -> 2482, 3.3V -> 4095
 */
void StartDAC(u16 dacValue)
{
    /* TODO: implement DC voltage output */
}


/*
 * Start DMA-driven waveform output on PA4.
 *
 * Steps:
 *   1. Configure DMA2 Channel 3 with current waveform buffer (s_strDAC1WaveBuf)
 *   2. Enable DAC DMA request (DAC_DMACmd)
 *
 * The waveform will loop continuously via DMA circular mode.
 * Timer4 triggers DAC conversions at the sample rate set in InitDAC.
 */
void StartDACWave(void)
{
    /* TODO: start DMA-driven waveform output */
}


/*
 * Stop all DAC output.
 *
 * Steps:
 *   1. Disable DAC DMA request
 *   2. Disable DMA2 Channel 3
 *   3. Set DAC output to 0
 */
void StopDAC(void)
{
    /* TODO: stop DAC output */
}


/*
 * Switch waveform to triangle wave.
 * Get the triangle wave data address from Wave module and update buffer descriptor.
 */
void SetDACWaveTri(void)
{
    /* TODO: set waveform to triangle (100 points) */
}


/*
 * Switch waveform to sine wave.
 * Get the sine wave data address from Wave module and update buffer descriptor.
 */
void SetDACWaveSine(void)
{
    /* TODO: set waveform to sine (100 points) */
}
