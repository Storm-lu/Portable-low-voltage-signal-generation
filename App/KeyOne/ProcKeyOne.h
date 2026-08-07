#ifndef _PROC_KEY_ONE_H_
#define _PROC_KEY_ONE_H_


#include "DataType.h"


/*
 * Initialize key processing logic (currently empty, reserved for future use).
 */
void  InitProcKeyOne(void);

/*
 * KEY1 press-down callback.
 * Called by KeyOne.c scan framework when KEY1 is pressed.
 *
 * Signal Output page: toggle output start/stop (OutputChange).
 * Settings page: move cursor to next setting item (ParamChange).
 */
void  ProcKeyDownKey1(void);

/*
 * KEY1 release callback (currently unused).
 */
void  ProcKeyUpKey1(void);

/*
 * KEY2 press-down callback.
 *
 * Settings page: increment current setting value (ValueChange).
 * Other pages: no action.
 */
void  ProcKeyDownKey2(void);

/*
 * KEY2 release callback (currently unused).
 */
void  ProcKeyUpKey2(void);

/*
 * KEY3 press-down callback.
 *
 * Settings page: start long-press timer (2s countdown).
 * Other pages: short press -> switch to next page (DisplayChange).
 */
void  ProcKeyDownKey3(void);

/*
 * KEY3 release callback.
 *
 * If long-press was reached (2s): save settings (SaveSettings) or switch page.
 * If short press: no additional action (page switch already handled on press).
 */
void  ProcKeyUpKey3(void);

/*
 * KEY3 long-press checker, called every 2ms from Proc2msTask().
 *
 * Increments counter while KEY3 is held. At 1000 ticks (2 seconds),
 * sets the longPress flag which triggers SaveSettings on release.
 */
void ProcKeyCheckKey3(void);

#endif
