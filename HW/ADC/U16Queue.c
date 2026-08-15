#include "U16Queue.h"


void  InitU16Queue(StructU16CirQue* pQue, u16* pBuf, i16 len)
{
  i16 i;

  pQue->front   = 0;
  pQue->rear    = 0;
  pQue->bufLen  = len;
  pQue->elemNum = 0;
  pQue->pBuffer = pBuf;

  for(i = 0; i < len; i++)
  {
    pQue->pBuffer[i]  = 0;
  }
}


void  ClearU16Queue(StructU16CirQue* pQue)
{
  pQue->front   = 0;
  pQue->rear    = 0;
  pQue->elemNum = 0;
}


u8    U16QueueEmpty(StructU16CirQue* pQue)
{
  return(0 == pQue->elemNum);
}


i16   U16QueueLength(StructU16CirQue* pQue)
{
  return(pQue->elemNum);
}


i16 EnU16Queue(StructU16CirQue* pQue, u16* pInput, i16 len)
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


i16 DeU16Queue(StructU16CirQue* pQue, u16* pOutput, i16 len)
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
