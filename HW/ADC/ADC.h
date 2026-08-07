#ifndef _ADC_H_
#define _ADC_H_


#include "DataType.h"


#define ADC1_BUF_SIZE 100
#define ADC_BUF_SIZE 128


void InitADC(void);

u16* GetADCBuf(void);
u16 GetADCLatest(void);

void GetADCStats(u16 *pAvg, u16 *pMax, u16 *pMin, u16 *pP2P);

u8   WriteADCBuf(u16 d);
u8   ReadADCBuf(u16 *p);

#endif
