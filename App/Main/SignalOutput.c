#include "Main.h"
#include "stm32f10x_conf.h"
#include "DataType.h"
#include "SignalOutput.h"
#include "DAC.h"
#include "PWM.h"
#include "Settings.h"

static SignalOutputType signal_mode;
static u8 signal_state = 0;

/*
 * Initialize signal output module.
 *
 * Read the saved mode from Settings (Flash-loaded):
 *   signal_mode = GetSettingsSignalMode();
 *   signal_state = 0;  (output stopped at startup)
 */
void InitSignalOutput(void){
    /* TODO: Read mode from Settings, set state to stopped. */
}

/*
 * Display the Signal Output page on OLED.
 *
 * Layout:
 *   Line 0 (y=0):   "Signal Output:"
 *   Line 1 (y=16):  "M:" + mode name
 *                   DC_1V -> "DC 1.0V", DC_2V -> "DC 2.0V"
 *                   TRIANGLE -> "TRI 100Hz", SINE -> "SINE 100Hz"
 *                   PWM -> "PWM"
 *   Line 2 (y=32):  "S:" + "RUNNING" or "STOPPED" (based on signal_state)
 *   Line 3 (y=48):  "KEY1: Start/Stop"
 *
 * Remember: OLEDClear() at start, OLEDRefreshGRAM() at end.
 */
void DisplaySignalOutput(void){
    /* TODO: Implement display layout as described above. */
}

/*
 * Toggle output start/stop (KEY1 action on Signal Output page).
 *
 * If currently running (signal_state==1):
 *   - Stop: StopDAC(), StopPWM(), signal_state=0
 *
 * If currently stopped (signal_state==0):
 *   - Start based on mode:
 *     TRIANGLE/SINE -> StartDACWave()
 *     PWM            -> StartPWM(GetSettingsPWMFreq(), GetSettingsPWMDuty())
 *     DC_1V          -> StartDAC(1241)   (1241/4095 * 3.3V ~ 1.0V)
 *     DC_2V          -> StartDAC(2482)   (2482/4095 * 3.3V ~ 2.0V)
 *   - signal_state=1
 */
void OutputChange(void){
    /* TODO: Implement start/stop toggle as described above. */
}

/*
 * Switch signal output type and restart output (called from Settings save).
 *
 * Steps:
 *   1. If currently running: stop everything (StopDAC, StopPWM), set state=0
 *   2. Set signal_mode = type
 *   3. Start the new mode:
 *      TRIANGLE -> SetDACWaveTri() + StartDACWave()
 *      SINE     -> SetDACWaveSine() + StartDACWave()
 *      PWM      -> StartPWM(GetSettingsPWMFreq(), GetSettingsPWMDuty())
 *      DC_1V    -> StartDAC(1241)
 *      DC_2V    -> StartDAC(2482)
 *   4. signal_state = 1
 *
 * Note: For TRIANGLE/SINE, must call BOTH SetDACWave* AND StartDACWave.
 *       SetDACWaveTri/Sine only sets the buffer pointer, StartDACWave starts DMA.
 */
void SetSignalOutputType(SignalOutputType type){
    /* TODO: Implement mode switch + restart as described above. */
}

/*
 * Get current signal output type.
 * Used by Proc1SecTask for USART output and by Settings.c.
 */
SignalOutputType GetSignalOutputType(void){
    return signal_mode;
}
