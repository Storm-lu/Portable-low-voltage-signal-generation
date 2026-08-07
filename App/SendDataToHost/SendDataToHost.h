#ifndef _SEND_DATA_TO_HOST_H_
#define _SEND_DATA_TO_HOST_H_


#include "DataType.h"


void  InitSendDataToHost(void);
void  SendAckPack(u8 moduleId, u8 secondId, u8 ackMsg);

void  SendWaveToHost(u8* pWaveData);

#endif
