#include "SysTick.h"
#include "stm32f10x_conf.h"


static  __IO  u32 s_iTimDelayCnt = 0;


static  void TimDelayDec(void);


static  void TimDelayDec(void)
{
  if(s_iTimDelayCnt != 0)
  {
    s_iTimDelayCnt--;
  }
}


void  SysTick_Handler(void)
{
  TimDelayDec();
}


void InitSysTick( void )
{
  if (SysTick_Config(SystemCoreClock / 1000))
  {
    while(1)
    {

    }
  }
}


void  DelayNms(__IO u32 nms)
{
  s_iTimDelayCnt = nms;

  while(s_iTimDelayCnt != 0)
  {

  }
}


void  DelayNus(__IO u32 nus)
{
  u32 s_iTimCnt = nus;
  u16 i;

  while(s_iTimCnt != 0)
  {
    for(i = 0; i < 7; i++)
    {

    }

    s_iTimCnt--;
  }
}
