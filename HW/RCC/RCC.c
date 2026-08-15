#include "RCC.h"
#include "stm32f10x_conf.h"


static  void  ConfigRCC(void);


static void ConfigRCC(void)
{
  ErrorStatus HSEStartUpStatus;

  RCC_DeInit();

  RCC_HSEConfig(RCC_HSE_ON);

  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    FLASH_SetLatency(FLASH_Latency_2);

    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    RCC_PCLK2Config(RCC_HCLK_Div1);

    RCC_PCLK1Config(RCC_HCLK_Div2);


    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    RCC_PLLCmd(ENABLE);

    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {

    }


    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);


    while(RCC_GetSYSCLKSource() != 0x08)
    {

    }
  }
}


void InitRCC(void)
{
  ConfigRCC();
}
