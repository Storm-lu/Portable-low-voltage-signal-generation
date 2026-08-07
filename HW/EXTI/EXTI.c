#include "EXTI.h"
#include "stm32f10x_conf.h"


static void ConfigEXTIGPIO(void);
static void ConfigEXTI(void);


static void ConfigEXTIGPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;


  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);


  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure);


  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure);


  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}


static void ConfigEXTI(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;


  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);


  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {

    GPIO_WriteBit(GPIOC, GPIO_Pin_4, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_4)));

    GPIO_WriteBit(GPIOC, GPIO_Pin_5, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_5)));
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}


void EXTI1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
  {

    GPIO_WriteBit(GPIOC, GPIO_Pin_4, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_4)));
    EXTI_ClearITPendingBit(EXTI_Line1);
  }
}


void EXTI2_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line2) != RESET)
  {

    GPIO_WriteBit(GPIOC, GPIO_Pin_5, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_5)));
    EXTI_ClearITPendingBit(EXTI_Line2);
  }
}


void  InitEXTI(void)
{
  ConfigEXTIGPIO();
  ConfigEXTI();
}
