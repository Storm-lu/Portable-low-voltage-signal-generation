#include "Main.h"
#include "Display.h"
#include "SignalMeasure.h"
#include "SignalOutput.h"
#include "Settings.h"

DisplayState displayState;
static u8 s_displayCnt = 0;

/*
 * Initialize the display system.
 *
 * Steps:
 *   1. Set initial page to DISPLAY_SIGNAL_MEASURE
 *   2. Reset frame counter s_displayCnt = 0
 *   3. Call InitSignalMeasure()
 *   4. Call InitSettings()       -- loads params from Flash (MUST be before SignalOutput)
 *   5. Call InitSignalOutput()   -- reads mode from Settings via GetSettingsSignalMode()
 *
 * Order is critical: Settings must load from Flash before SignalOutput reads the mode.
 */
void InitDisplay(void)
{
    /* TODO: Implement as described above. */
}

/*
 * Display process - called every 2ms from Proc2msTask.
 *
 * Throttle: only refresh every 50 calls (50 * 2ms = 100ms = 10Hz).
 * This prevents OLED flickering from too-frequent updates.
 *
 * When the counter hits 50, reset to 0 and call the appropriate
 * Display function based on current displayState:
 *   DISPLAY_SIGNAL_MEASURE -> DisplaySignalMeasure()
 *   DISPLAY_SIGNAL_OUTPUT  -> DisplaySignalOutput()
 *   DISPLAY_SETTINGS       -> DisplaySettings()
 */
void DisplayProcess(void){
    /* TODO:
     *   s_displayCnt++;
     *   if(s_displayCnt < 50) return;
     *   s_displayCnt = 0;
     *   switch(displayState) { ... call appropriate Display function ... } */
}

/*
 * Switch to the next display page (cyclic).
 *
 * Order: Signal Measure -> Signal Output -> Settings -> Signal Measure
 *
 * Note: In Settings page, short KEY3 press does NOT call this function.
 * Only long-press (2s) exits Settings via SaveSettings().
 */
void DisplayChange(void){
    /* TODO: switch on displayState, advance to next page cyclically. */
}

/*
 * Print current display state name via USART (for debugging).
 * Called from Proc1SecTask().
 */
void PrintState(void){
    switch(displayState){
        case DISPLAY_SIGNAL_MEASURE:
            printf("Signal Measure\r\n");
            break;
        case DISPLAY_SIGNAL_OUTPUT:
            printf("Signal Output\r\n");
            break;
        case DISPLAY_SETTINGS:
            printf("Settings\r\n");
            break;
    }
}

/*
 * Get current display state.
 * Used by ProcKeyOne.c to determine which key actions to perform.
 */
DisplayState GetDisplayState(void)
{
  return displayState;
}
