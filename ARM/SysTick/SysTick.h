#ifndef _SYS_TICK_H_
#define _SYS_TICK_H_


#include "DataType.h"
#include "stm32f10x.h"


void  InitSysTick(void);
void  DelayNus(__IO u32 nus);
void  DelayNms(__IO u32 nms);

#endif
