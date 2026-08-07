#include "SendDataToHost.h"
#include "PackUnpack.h"
#include "UART1.h"


static  void  SendPackToHost(StructPackType* pPackSent);


static  void  SendPackToHost(StructPackType* pPackSent)
{
  u8  packValid = 0;

  packValid = PackData(pPackSent);

  if(0 < packValid)
  {
    WriteUART1((u8*)pPackSent, 10);
  }
}


void  InitSendDataToHost(void)
{

}


void SendAckPack(u8 moduleId, u8 secondId, u8 ackMsg)
{
  StructPackType pt;

  pt.packModuleId = MODULE_SYS;
  pt.packSecondId = DAT_CMD_ACK;
  pt.arrData[0] = moduleId;
  pt.arrData[1] = secondId;
  pt.arrData[2] = ackMsg;
  pt.arrData[3] = 0;
  pt.arrData[4] = 0;
  pt.arrData[5] = 0;

  SendPackToHost(&pt);
}


void  SendWaveToHost(u8* pWaveData)
{
  StructPackType  pt;

  pt.packModuleId = MODULE_WAVE;
  pt.packSecondId = DAT_WAVE_WDATA;
  pt.arrData[0] = pWaveData[0];
  pt.arrData[1] = pWaveData[1];
  pt.arrData[2] = pWaveData[2];
  pt.arrData[3] = pWaveData[3];
  pt.arrData[4] = pWaveData[4];
  pt.arrData[5] = 0;

  SendPackToHost(&pt);
}
