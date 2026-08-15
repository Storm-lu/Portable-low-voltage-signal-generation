#include "PackUnpack.h"


static StructPackType s_ptPack;
static u8       s_iPackLen;
static u8       s_iGotPackId;
static u8       s_iRestByteNum;


static  void  PackWithCheckSum(u8* pPack);
static  u8    UnpackWithCheckSum(u8* pPack);


static void PackWithCheckSum(u8* pPack)
{
  u8  i;
  u8  dataHead;
  u8  checkSum;

  checkSum = *(pPack);
  dataHead = 0;

  for(i = 8; i > 1; i --)
  {

    dataHead <<= 1;


    dataHead |= (*(pPack + i) & 0x80) >> 7;


    *(pPack + i) = *(pPack + i) | 0x80;


    checkSum += *(pPack+i);
  }


  *(pPack+1) = dataHead | 0x80;


  checkSum += *(pPack+1);


  *(pPack+9) = checkSum | 0x80;
}


static u8  UnpackWithCheckSum(u8* pPack)
{
  u8  i;
  u8  dataHead;
  u8  checkSum;

  checkSum  = *(pPack);

  dataHead = *(pPack + 1);
  checkSum += dataHead;

  for(i = 2; i < 9; i++)
  {
    checkSum += *(pPack + i);


    *(pPack + i) = (*(pPack + i) & 0x7f) | ((dataHead & 0x1) << 7);

    dataHead >>= 1;
  }


  if((checkSum & 0x7f ) != ((*(pPack + 9)) & 0x7f))
  {
    return 0;
  }

  return 1;
}


void  InitPackUnpack(void)
{
  i16 i;

  s_ptPack.packModuleId  = 0;
  s_ptPack.packHead      = 0;
  s_ptPack.packSecondId  = 0;

  for(i = 0; i < 7; i++)
  {
    s_ptPack.arrData[i]   = 0;
  }

  s_ptPack.checkSum  = 0;

  s_iPackLen     = 0;
  s_iGotPackId   = 0;
  s_iRestByteNum = 0;
}


u8  PackData(StructPackType* pPT)
{
  u8 valid = 0;

  if(pPT->packModuleId < 0x80)
  {
    valid = 1;
    PackWithCheckSum((u8 *)pPT);
  }

  return(valid);
}


u8  UnPackData(u8 data)
{
  u8 findPack = 0;
  u8* pBuf;

  pBuf = (u8*)&s_ptPack;

  if(s_iGotPackId)
  {
    if(0x80 <= data)
    {

      pBuf[s_iPackLen] = data;
      s_iPackLen++;
      s_iRestByteNum--;

      if (0 >= s_iRestByteNum && 10 == s_iPackLen)
      {
        findPack = UnpackWithCheckSum(pBuf);
        s_iGotPackId = 0;
      }
    }
    else
    {
      s_iGotPackId = 0;
    }
  }
  else if( data < 0x80 )
  {
    s_iRestByteNum  = 9;
    s_iPackLen      = 1;
    s_ptPack.packModuleId = data;
    s_iGotPackId    = 1;
  }

  return findPack;
}


StructPackType  GetUnPackRslt(void)
{
  return(s_ptPack);
}
