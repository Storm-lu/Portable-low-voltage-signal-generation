#include "NVIC.h"
#include "stm32f10x_conf.h"


static  void ConfigNVIC(void);


static  void ConfigNVIC(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}


void InitNVIC(void)
{
  ConfigNVIC();
}
