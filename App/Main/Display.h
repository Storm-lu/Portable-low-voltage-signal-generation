#ifndef __DISPLAY_H__
#define __DISPLAY_H__

/*
 * Display page states for the three-page state machine.
 * Cycle order: MEASURE -> OUTPUT -> SETTINGS -> MEASURE
 */
typedef enum
{
  DISPLAY_SIGNAL_MEASURE,   /* ADC waveform + frequency/duty display */
  DISPLAY_SIGNAL_OUTPUT,    /* Signal mode + start/stop status */
  DISPLAY_SETTINGS          /* Parameter selection + Flash save */
} DisplayState;

/* Current display page (extern for ProcKeyOne.c access). */
extern DisplayState displayState;

/*
 * Initialize display system.
 * Sets initial page, calls InitSignalMeasure, InitSettings, InitSignalOutput.
 * Settings MUST init before SignalOutput (Flash data needed).
 */
void InitDisplay(void);

/*
 * Display process - called every 2ms.
 * Internally throttled to 10Hz (every 50 calls) to prevent flickering.
 * Dispatches to the appropriate page display function.
 */
void DisplayProcess(void);

/*
 * Advance to the next display page (cyclic).
 * Called by KEY3 short press (except in Settings page).
 */
void DisplayChange(void);

/*
 * Print current page name via USART for debugging.
 * Called from Proc1SecTask().
 */
void PrintState(void);

/*
 * Get current display state.
 * Used by ProcKeyOne.c to determine key action dispatch.
 */
DisplayState GetDisplayState(void);

#endif
