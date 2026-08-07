#include "Main.h"
#include "stm32f10x_conf.h"
#include "DataType.h"
#include "SignalMeasure.h"
#include "ADC.h"
#include "Capture.h"
#include "OLED.h"

/* Waveform display area: left 80 pixels wide, top 48 pixels tall */
#define WAVE_X_START    0
#define WAVE_Y_START    0
#define WAVE_WIDTH      80
#define WAVE_HEIGHT     48

void InitSignalMeasure(void){
    /* Nothing to initialize currently. */
}

/*
 * Display the Signal Measure page on OLED.
 *
 * Layout (oscilloscope style):
 *
 *   +-------- Waveform Area --------+-- Text Area --+
 *   |                               | V:x.xV        |  y=0   (peak voltage)
 *   |  80 x 48 pixels with grid     | F:xxxH        |  y=16  (frequency)
 *   |  Auto-scaled waveform         | P:x.xV        |  y=32  (peak-to-peak)
 *   |                               | D:xx%         |  y=48  (duty cycle)
 *   +-------------------------------+---------------+
 *
 * Data sources:
 *   pADC = GetADCBuf()                      -- 128-point ADC buffer (Team A)
 *   GetADCStats(&avg, &maxV, &minV, &p2p)   -- statistics (Team A)
 *   freq = GetCaptureFreq()                 -- frequency in Hz (Team B)
 *   duty = GetCaptureDuty()                 -- duty 0~100, 0xFF=invalid (Team B)
 *
 * Voltage conversion (12-bit ADC, 3.3V reference):
 *   voltMV = maxV * 3300 / 4095    (peak voltage in millivolts)
 *   p2pMV  = p2p  * 3300 / 4095    (peak-to-peak in millivolts)
 *
 * Frequency display format:
 *   freq == 0        -> "--  "
 *   freq >= 1000     -> "x.xk" (if < 10kHz) or "xxxk" (if < 1000kHz) or ">1M"
 *   freq < 1000      -> "xxxH"
 *
 * Duty display: only shown if duty != 0xFF (valid signal)
 *
 * Drawing steps:
 *   1. OLEDClear()
 *   2. OLEDDrawGrid(WAVE_X_START, WAVE_Y_START, WAVE_WIDTH, WAVE_HEIGHT, 8, 4)
 *   3. OLEDDrawWaveFormEx(pADC, ADC_BUF_SIZE, ..., gain=0)  [auto-scale]
 *   4. Draw text labels and values in right column (x=80)
 *   5. OLEDRefreshGRAM()
 */
void DisplaySignalMeasure(void){
    /* TODO: Implement as described above.
     *
     * Key points:
     *   - Use maxV (not avg) for the V: display (shows peak, not average)
     *   - Auto-scale waveform (gain=0) so any signal amplitude fills the area
     *   - Right column starts at x=80, each line is 16px apart
     *   - OLEDShowNum for numbers, OLEDShowString for labels/units */
}
