#ifndef _UART1_H_
#define _UART1_H_


#include <stdio.h>
#include "DataType.h"


#define UART1_BUF_SIZE 100


void  InitUART1(u32 bound);
u8    WriteUART1(u8 *pBuf, u8 len);
u8    ReadUART1(u8 *pBuf, u8 len);

#endif
