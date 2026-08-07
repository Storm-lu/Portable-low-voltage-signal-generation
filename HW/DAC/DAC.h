#ifndef _DAC_H_
#define _DAC_H_


#include "DataType.h"


typedef struct
{
  u32 waveBufAddr;
  u32 waveBufSize;
}StructDACWave;

void StartDAC(u16 dacValue);
void StartDACWave();
void StopDAC(void);
void  InitDAC(void);
void  SetDACWave();

void SetDACWaveTri(void);
void SetDACWaveSine(void);

#endif
