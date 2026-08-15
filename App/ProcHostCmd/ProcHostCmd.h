#ifndef _PROC_HOST_CMD_H_
#define _PROC_HOST_CMD_H_


#include "DataType.h"


typedef enum{
  CMD_ACK_OK,
  CMD_ACK_CHECKSUM,
  CMD_ACK_LEN,
  CMD_ACK_BAD_CMD,
  CMD_ACK_PARAM_ERR,
  CMD_ACK_NOT_ACC
}EnumCmdAckType;


void  InitProcHostCmd(void);
void  ProcHostCmd(u8 recData);

#endif
