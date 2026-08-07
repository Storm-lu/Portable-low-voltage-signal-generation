#include "ProcHostCmd.h"
#include "PackUnpack.h"
#include "SendDataToHost.h"
#include "DAC.h"
#include "Wave.h"


static u8  OnGenWave(u8* pMsg);


static u8 OnGenWave(u8* pMsg)
{
  StructDACWave wave;

  if(pMsg[0] == 0x00)
  {
    wave.waveBufAddr  = (u32)GetSineWave100PointAddr();
  }
  else if(pMsg[0] == 0x01)
  {
    wave.waveBufAddr  = (u32)GetTriWave100PointAddr();
  }
  else if(pMsg[0] == 0x02)
  {
    wave.waveBufAddr  = (u32)GetRectWave100PointAddr();
  }

  wave.waveBufSize  = 100;

  SetDACWave(wave);

  return(CMD_ACK_OK);
}


void  InitProcHostCmd(void)
{

}


void ProcHostCmd(u8 recData)
{
  u8 ack;
  StructPackType pack;

  while(UnPackData(recData))
  {
    pack = GetUnPackRslt();

    switch(pack.packModuleId)
    {
      case MODULE_WAVE:
        ack = OnGenWave(pack.arrData);
        SendAckPack(MODULE_WAVE, CMD_GEN_WAVE, ack);
        break;
      default:
        break;
    }
  }
}
