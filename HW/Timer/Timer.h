#ifndef _TIMER_H_
#define _TIMER_H_


#include "DataType.h"


void  InitTimer(void);

u8    Get2msFlag(void);
void  Clr2msFlag(void);

u8    Get1SecFlag(void);
void  Clr1SecFlag(void);

#endif
