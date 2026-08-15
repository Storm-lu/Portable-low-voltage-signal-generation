#include "Timer.h"
#include "stm32f10x_tim.h"


static  u8  s_i2msFlag  = FALSE;
static  u8  s_i1secFlag = FALSE;


static  void  ConfigTimer2(u16 arr, u16 psc);
static  void  ConfigTimer5(u16 arr, u16 psc);


static  void ConfigTimer2(u16 arr, u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;


  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);


  TIM_TimeBaseStructure.TIM_Period        = arr;
  TIM_TimeBaseStructure.TIM_Prescaler     = psc;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);


  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  TIM_Cmd(TIM2, ENABLE);
}


static  void ConfigTimer5(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;


  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);


  TIM_TimeBaseStructure.TIM_Period        = arr;
  TIM_TimeBaseStructure.TIM_Prescaler     = psc;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

  TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);


  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  TIM_Cmd(TIM5, ENABLE);
}


void TIM2_IRQHandler(void)
{
  static  u16 s_iCnt2 = 0;

  if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
  {
    TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
  }

  s_iCnt2++;

  if(s_iCnt2 >= 2)
  {
    s_iCnt2 = 0;
    s_i2msFlag = TRUE;
  }
}


void TIM5_IRQHandler(void)
{
  static  i16 s_iCnt1000  = 0;

  if (TIM_GetITStatus(TIM5, TIM_IT_Update) == SET)
  {
    TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);
  }

  s_iCnt1000++;

  if(s_iCnt1000 >= 1000)
  {
    s_iCnt1000 = 0;
    s_i1secFlag = TRUE;
  }
}


void InitTimer(void)
{
  ConfigTimer2(999, 71);
  ConfigTimer5(999, 71);
}


u8  Get2msFlag(void)
{
  return(s_i2msFlag);
}


void  Clr2msFlag(void)
{
  s_i2msFlag = FALSE;
}


u8  Get1SecFlag(void)
{
  return(s_i1secFlag);
}


void  Clr1SecFlag(void)
{
  s_i1secFlag = FALSE;
}
