#include "Queue.h"


void  InitQueue(StructCirQue* pQue, DATA_TYPE* pBuf, i16 len)
{
  i16 i;

  pQue->front   = 0;
  pQue->rear    = 0;
  pQue->bufLen  = len;
  pQue->elemNum = 0;
  pQue->pBuffer = pBuf;

  for(i = 0; i < len; i++)
  {
    pQue->pBuffer[i] = 0;
  }
}


void  ClearQueue(StructCirQue* pQue)
{
  pQue->front   = 0;
  pQue->rear    = 0;
  pQue->elemNum = 0;
}


u8    QueueEmpty(StructCirQue* pQue)
{
  return(0 == pQue->elemNum);
}


i16   QueueLength(StructCirQue* pQue)
{
  return(pQue->elemNum);
}


i16 EnQueue(StructCirQue* pQue, DATA_TYPE* pInput, i16 len)
{
  i16 wLen = 0;

  while((pQue->elemNum < pQue->bufLen) && (wLen < len))
  {
    pQue->pBuffer[pQue->rear] = pInput[wLen];
    pQue->rear++;

    if(pQue->rear >= pQue->bufLen)
    {
      pQue->rear = 0;
    }

    wLen++;
    pQue->elemNum++;
  }

  return wLen;
}


i16 DeQueue(StructCirQue* pQue, DATA_TYPE* pOutput, i16 len)
{
  i16 rLen = 0;

  while((pQue->elemNum > 0) && (rLen < len))
  {
    pOutput[rLen] = pQue->pBuffer[pQue->front];
    pQue->front++;

    if( pQue->front >= pQue->bufLen )
    {
      pQue->front = 0;
    }

    rLen++;
    pQue->elemNum--;
  }

  return rLen;
}
