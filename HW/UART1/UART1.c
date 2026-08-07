#include "UART1.h"
#include "stm32f10x_conf.h"
#include "Queue.h"


typedef enum
{
  UART_STATE_OFF,
  UART_STATE_ON,
  UART_STATE_MAX
}EnumUARTState;


static  StructCirQue s_structUARTSendCirQue;
static  StructCirQue s_structUARTRecCirQue;
static  u8  s_arrSendBuf[UART1_BUF_SIZE];
static  u8  s_arrRecBuf[UART1_BUF_SIZE];

static  u8  s_iUARTTxSts;


static  void  InitUARTBuf(void);
static  u8    WriteReceiveBuf(u8 d);
static  u8    ReadSendBuf(u8 *p);

static  void  ConfigUART(u32 bound);
static  void  EnableUARTTx(void);

static  void  SendCharUsedByFputc(u16 ch);


static  void  InitUARTBuf(void)
{
  i16 i;

  for(i = 0; i < UART1_BUF_SIZE; i++)
  {
    s_arrSendBuf[i] = 0;
    s_arrRecBuf[i]  = 0;
  }

  InitQueue(&s_structUARTSendCirQue, s_arrSendBuf, UART1_BUF_SIZE);
  InitQueue(&s_structUARTRecCirQue,  s_arrRecBuf,  UART1_BUF_SIZE);
}


static  u8  WriteReceiveBuf(u8 d)
{
  u8 ok = 0;

  ok = EnQueue(&s_structUARTRecCirQue, &d, 1);

  return ok;
}


static  u8  ReadSendBuf(u8 *p)
{
  u8 ok = 0;

  ok = DeQueue(&s_structUARTSendCirQue, p, 1);

  return ok;
}


static  void  ConfigUART(u32 bound)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;


  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);


  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


  USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate   = bound;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits   = USART_StopBits_1;
  USART_InitStructure.USART_Parity     = USART_Parity_No;
  USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_Init(USART1, &USART_InitStructure);


  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART1, USART_IT_TXE,  ENABLE);
  USART_Cmd(USART1, ENABLE);

  s_iUARTTxSts = UART_STATE_OFF;
}


static  void  EnableUARTTx(void)
{
  s_iUARTTxSts = UART_STATE_ON;

  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}


static  void  SendCharUsedByFputc(u16 ch)
{
  USART_SendData(USART1, (u8)ch);


  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {

  }
}


void USART1_IRQHandler(void)
{
  u8  uData = 0;

  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    NVIC_ClearPendingIRQ(USART1_IRQn);
    uData = USART_ReceiveData(USART1);

    WriteReceiveBuf(uData);
  }

  if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) == SET)
  {
    USART_ClearFlag(USART1, USART_FLAG_ORE);
    USART_ReceiveData(USART1);
  }

  if(USART_GetITStatus(USART1, USART_IT_TXE)!= RESET)
  {
    USART_ClearITPendingBit(USART1, USART_IT_TXE);
    NVIC_ClearPendingIRQ(USART1_IRQn);

    ReadSendBuf(&uData);

    USART_SendData(USART1, uData);

    if(QueueEmpty(&s_structUARTSendCirQue))
    {
      s_iUARTTxSts = UART_STATE_OFF;
      USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    }
  }
}


void InitUART1(u32 bound)
{
  InitUARTBuf();

  ConfigUART(bound);
}


u8  WriteUART1(u8 *pBuf, u8 len)
{
  u8 wLen = 0;

  wLen = EnQueue(&s_structUARTSendCirQue, pBuf, len);

  if(wLen < UART1_BUF_SIZE)
  {
    if(s_iUARTTxSts == UART_STATE_OFF)
    {
      EnableUARTTx();
    }
  }

  return wLen;
}


u8  ReadUART1(u8 *pBuf, u8 len)
{
  u8 rLen = 0;

  rLen = DeQueue(&s_structUARTRecCirQue, pBuf, len);

  return rLen;
}


int fputc(int ch, FILE* f)
{
  SendCharUsedByFputc((u8) ch);

  return ch;
}
