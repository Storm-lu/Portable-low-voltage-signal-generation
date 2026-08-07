#ifndef _PACK_UNPACK_H_
#define _PACK_UNPACK_H_


#include "DataType.h"
#include "UART1.h"


typedef struct
{
  u8 packModuleId;
  u8 packHead;
  u8 packSecondId;
  u8 arrData[6];
  u8 checkSum;
}StructPackType;


typedef enum
{
  MODULE_SYS     = 0x01,
  MODULE_ECG     = 0x10,
  MODULE_RESP    = 0x11,
  MODULE_TEMP    = 0x12,
  MODULE_SPO2    = 0x13,
  MODULE_NBP     = 0x14,

  MODULE_WAVE    = 0x71,

  MAX_MODULE_ID  = 0x80
}EnumPackID;


typedef enum
{
  DAT_RST         = 0x01,
  DAT_SYS_STS     = 0x02,
  DAT_SELF_CHECK  = 0x03,
  DAT_CMD_ACK     = 0x04,

  CMD_RST_ACK     = 0x80,
  CMD_GET_POST_RSLT = 0x81,
  CMD_PAT_TYPE    = 0x90,
}EnumSysSecondID;


typedef enum
{
  DAT_ECG_WAVE    = 0x02,
  DAT_ECG_LEAD    = 0x03,
  DAT_ECG_HR      = 0x04,
  DAT_ST          = 0x05,
  DAT_ST_PAT      = 0x06,

  CMD_LEAD_SYS    = 0x80,
  CMD_LEAD_TYPE   = 0x81,
  CMD_FILTER_MODE = 0x82,
  CMD_ECG_GAIN    = 0x83,
  CMD_ECG_CAL     = 0x84,
  CMD_ECG_TRA     = 0x85,
  CMD_ECG_PACE    = 0x86,
  CMD_ECG_ST_ISO  = 0x87,
  CMD_ECG_CHANNEL = 0x88,
  CMD_ECG_LEADRN  = 0x89,
}EnumECGSecondID;


typedef enum
{
  DAT_RESP_WAVE   = 0x02,
  DAT_RESP_RR     = 0x03,
  DAT_RESP_APNEA  = 0x04,
  DAT_RESP_CVA    = 0x05,

  CMD_RESP_GAIN   = 0x80,
  CMD_RESP_APNEA  = 0x81,
}EnumRespSecondID;


typedef enum
{
  DAT_TEMP_DATA   = 0x02,

  CMD_TEMP        = 0x80,
}EnumTempSecondID;


typedef enum
{
  DAT_SPO2_WAVE   = 0x02,
  DAT_SPO2_DATA   = 0x03,

  CMD_SPO2        = 0x80,
}EnumSPO2SecondID;


typedef enum
{
  DAT_NBP_CUFPRE  = 0x02,
  DAT_NBP_END     = 0x03,
  DAT_NBP_RSLT1   = 0x04,
  DAT_NBP_RSLT2   = 0x05,
  DAT_NBP_STS     = 0x06,

  CMD_NBP_START   = 0x80,
  CMD_NBP_END     = 0x81,
  CMD_NBP_PERIOD  = 0x82,
  CMD_NBP_CALIB   = 0x83,
  CMD_NBP_RST     = 0x84,
  CMD_NBP_CHECK_LEAK  = 0x85,
  CMD_NBP_QUERY_STS   = 0x86,
  CMD_NBP_FIRST_PRE   = 0x87,
  CMD_NBP_CONT    = 0x88,
  CMD_NBP_RSLT    = 0x89,
}EnumNBPSecondID;


typedef enum
{
  DAT_WAVE_WDATA  = 0x01,

  CMD_GEN_WAVE    = 0x80,
}EnumWaveSecondID;


void  InitPackUnpack(void);
u8    PackData(StructPackType* pPT);
u8    UnPackData(u8 data);

StructPackType  GetUnPackRslt(void);

#endif
