#include "LED.h"
#include "stm32f10x_conf.h"


static  void  ConfigLEDGPIO(void);


static  void  ConfigLEDGPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;


  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET);

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_RESET);
}


void InitLED(void)
{
  ConfigLEDGPIO();
}


void LEDFlicker(u16 cnt)
{
  static u16 s_iCnt;

  s_iCnt++;

  if(s_iCnt >= cnt)
  {
    s_iCnt = 0;


    GPIO_WriteBit(GPIOC, GPIO_Pin_4, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_4)));


    GPIO_WriteBit(GPIOC, GPIO_Pin_5, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_5)));
  }
}


void SetLEDSts(u8 sts)
{
  if(sts & 0x01)
  {
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET);
  }
  else
  {
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);
  }

  if(sts & 0x02)
  {
    GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_SET);
  }
  else
  {
    GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_RESET);
  }
}
