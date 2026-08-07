#include "KeyOne.h"
#include "stm32f10x_conf.h"


#define KEY1    (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1))

#define KEY2    (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2))

#define KEY3    (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))


static  u8  s_arrKeyDownLevel[KEY_NAME_MAX];


static  void  ConfigKeyOneGPIO(void);


static  void  ConfigKeyOneGPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;


  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);


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


void InitKeyOne(void)
{
  ConfigKeyOneGPIO();

  s_arrKeyDownLevel[KEY_NAME_KEY1] = KEY_DOWN_LEVEL_KEY1;
  s_arrKeyDownLevel[KEY_NAME_KEY2] = KEY_DOWN_LEVEL_KEY2;
  s_arrKeyDownLevel[KEY_NAME_KEY3] = KEY_DOWN_LEVEL_KEY3;
}


void ScanKeyOne(u8 keyName, void(*OnKeyOneUp)(void), void(*OnKeyOneDown)(void))
{
  static  u8  s_arrKeyVal[KEY_NAME_MAX];
  static  u8  s_arrKeyFlag[KEY_NAME_MAX];

  s_arrKeyVal[keyName] = s_arrKeyVal[keyName] << 1;

  switch (keyName)
  {
    case KEY_NAME_KEY1:
      s_arrKeyVal[keyName] = s_arrKeyVal[keyName] | KEY1;
      break;
    case KEY_NAME_KEY2:
      s_arrKeyVal[keyName] = s_arrKeyVal[keyName] | KEY2;
      break;
    case KEY_NAME_KEY3:
      s_arrKeyVal[keyName] = s_arrKeyVal[keyName] | KEY3;
      break;
    default:
      break;
  }


  if(s_arrKeyVal[keyName] == s_arrKeyDownLevel[keyName] && s_arrKeyFlag[keyName] == TRUE)
  {
    (*OnKeyOneDown)();
    s_arrKeyFlag[keyName] = FALSE;
  }


  else if(s_arrKeyVal[keyName] == (u8)(~s_arrKeyDownLevel[keyName]) && s_arrKeyFlag[keyName] == FALSE)
  {
    (*OnKeyOneUp)();
    s_arrKeyFlag[keyName] = TRUE;
  }
}

u8 IsKeyPressed(u8 keyName)
{
  u8 val = 1;
  switch (keyName)
  {
    case KEY_NAME_KEY1:
      val = KEY1;
      break;
    case KEY_NAME_KEY2:
      val = KEY2;
      break;
    case KEY_NAME_KEY3:
      val = KEY3;
      break;
    default:
      break;
  }

  return (val == 0);
}