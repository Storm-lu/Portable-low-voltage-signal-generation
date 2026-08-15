#include "IWDG.h"
#include "stm32f10x_conf.h"


static  void ConifigIWDG(u8 prer, u16 rlr);


static void ConifigIWDG(u8 prer, u16 rlr)
{

  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

  IWDG_SetPrescaler(prer);

  IWDG_SetReload(rlr);

  IWDG_ReloadCounter();

  IWDG_Enable();
}


void InitIWDG(void)
{
  ConifigIWDG(IWDG_Prescaler_64, 624);
}


void FeedIWDG(void)
{
  IWDG_ReloadCounter();
}
