#ifndef _QUEUE_H_
#define _QUEUE_H_


#include "DataType.h"


typedef u8  DATA_TYPE;


typedef struct
{
  i16       front;
  i16       rear;
  i16       bufLen;
  i16       elemNum;
  DATA_TYPE *pBuffer;
}StructCirQue;


void  InitQueue(StructCirQue* pQue, DATA_TYPE* pBuf, i16 len);
void  ClearQueue(StructCirQue* pQue);
u8    QueueEmpty(StructCirQue* pQue);
i16   QueueLength(StructCirQue* pQue);
i16   EnQueue(StructCirQue* pQue, DATA_TYPE* pInput, i16 len);
i16   DeQueue(StructCirQue* pQue, DATA_TYPE* pOutput, i16 len);

#endif
