#ifndef _U16_QUEUE_H_
#define _U16_QUEUE_H_


#include "DataType.h"


typedef struct
{
  i16 front;
  i16 rear;
  i16 bufLen;
  i16 elemNum;
  u16 *pBuffer;
}StructU16CirQue;


void  InitU16Queue(StructU16CirQue* pQue, u16* pBuf, i16 len);
void  ClearU16Queue(StructU16CirQue* pQue);
u8    U16QueueEmpty(StructU16CirQue* pQue);
i16   U16QueueLength(StructU16CirQue* pQue);
i16   EnU16Queue(StructU16CirQue* pQue, u16* pInput, i16 len);
i16   DeU16Queue(StructU16CirQue* pQue, u16* pOutput, i16 len);

#endif
