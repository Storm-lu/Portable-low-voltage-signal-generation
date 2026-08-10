/*********************************************************************************************************
* 模块名称：InputCapture.c
* 摘    要：输入捕获频率测量模块，基于TIM1 CH1(PA8)，上升沿+下降沿双捕获测量脉冲频率+占空比
* 当前版本：1.2.0
* 作    者：成员C（信号测量模块）
* 完成日期：2026年08月
* 内    容：基础任务8——数字脉冲频率测量
*           测量范围：100Hz ~ 10kHz
*           无信号超时：1s（超时后清除结果）
*           进阶：上升沿+下降沿双捕获实现占空比测量
* 注    意：定时器分频PSC=71，计数频率1MHz；测量误差须 ≤ 2%
*           修复：加入定时器溢出计数，解决16位计数器跨溢出测量异常问题
**********************************************************************************************************
* 取代版本：TIM4 PB6版本
* 修改内容：底层定时器更换TIM1 CH1 PA8；修复溢出处理；上层API完全兼容无需改动
*********************************************************************************************************/

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "InputCapture.h"
#include "stm32f10x.h"

/*********************************************************************************************************
*                                              内部变量
*********************************************************************************************************/
static u32  s_freqValue    = 0;   //当前频率值(Hz)
static u8   s_hasSignal    = 0;   //有效信号标志
static u16  s_timeoutCnt   = 0;   //无信号超时计数(ms)
static u8   s_captureDuty  = 0;   //占空比(%)

static u32  s_lastRisingTs = 0;   //上一次上升沿绝对时间戳(含溢出)
static u32  s_curRisingTs  = 0;   //本次上升沿绝对时间戳
static u32  s_fallingTs    = 0;   //下降沿绝对时间戳
static u32  s_overflowCnt  = 0;   //定时器溢出次数

//捕获状态机
typedef enum
{
  CAP_WAIT_RISING,
  CAP_WAIT_FALLING
}CAP_STATE;
static CAP_STATE s_capState = CAP_WAIT_RISING;

/*********************************************************************************************************
*                                              API函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称：InitInputCapture
* 函数功能：初始化输入捕获模块（TIM1 CH1，PA8）
* 输入参数：void
* 输出参数：void
* 返 回 值：void
* 创建日期：2026年08月
* 注    意：PSC=71，计数频率1MHz；TIM1高级定时器，捕获/溢出分为两路中断
*********************************************************************************************************/
 //TODO(成员C):
  //1. 使能TIM4与GPIOB时钟
  //2. 配置PB6为浮空/上拉输入
  //3. 配置TIM4时基：PSC=71, ARR=0xFFFF
  //4. 配置TIM4_CH1为输入捕获，上升沿触发
  //5. 使能捕获/更新中断，启动定时器
  
void  InitInputCapture(void)
{
  GPIO_InitTypeDef        GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_ICInitTypeDef       TIM_ICInitStructure;
  NVIC_InitTypeDef        NVIC_InitStructure;

  //1. 开启时钟：TIM1、GPIOA都属于APB2
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  //2. PA8(TIM1_CH1) 浮空输入
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //3. TIM1时基配置 72M/(71+1)=1MHz计数时钟
  TIM_TimeBaseStructure.TIM_Period        = 0xFFFF;
  TIM_TimeBaseStructure.TIM_Prescaler     = 71;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  //4. TIM1_CH1 初始上升沿输入捕获
  TIM_ICInitStructure.TIM_Channel     = TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter    = 0x00;
  TIM_ICInit(TIM1, &TIM_ICInitStructure);

  //===== 高级定时器TIM1：两路独立中断，分开配置NVIC =====
  //① 捕获比较中断 TIM1_CC_IRQn
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  //② 更新溢出中断 TIM1_UP_IRQn
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  //使能捕获中断CC1、溢出更新中断
  TIM_ITConfig(TIM1, TIM_IT_CC1 | TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM1, ENABLE);

  //内部变量初始化
  s_freqValue    = 0;
  s_hasSignal    = 0;
  s_timeoutCnt   = 0;
  s_captureDuty  = 0;
  s_overflowCnt  = 0;
  s_lastRisingTs = 0;
  s_curRisingTs  = 0;
  s_fallingTs    = 0;
  s_capState     = CAP_WAIT_RISING;
}

/*********************************************************************************************************
* 函数名称：Capture_GetFreq
* 函数功能：获取频率值(Hz)，返回0表示无信号
* 输入参数：void
* 输出参数：void
* 返 回 值：测量频率
* 创建日期：2026年08月
* 注    意：f = 1000000 / tick_diff；限制区间100Hz~10kHz
*********************************************************************************************************/
u32  Capture_GetFreq(void)
{
  return s_freqValue;
}

/*********************************************************************************************************
* 函数名称：Capture_HasSignal
* 函数功能：是否有有效信号 1=有 0=无
* 输入参数：void
* 输出参数：void
* 返 回 值：信号标志
* 创建日期：2026年08月
*********************************************************************************************************/
u8  Capture_HasSignal(void)
{
  return s_hasSignal;
}

/*********************************************************************************************************
* 函数名称：Capture_GetDuty
* 函数功能：获取占空比(%)，0~100；进阶任务【已实现】
* 输入参数：void
* 输出参数：void
* 返 回 值：占空比数值
* 创建日期：2026年08月
*********************************************************************************************************/
u8  Capture_GetDuty(void)
{
  return s_captureDuty;
}

/*********************************************************************************************************
* 函数名称：Capture_ProcessTimeout
* 函数功能：1ms周期调用，处理无信号超时清零
* 输入参数：void
* 输出参数：void
* 返 回 值：void
* 创建日期：2026年08月
* 注    意：超时1s无捕获则清除频率结果、标记无信号、复位状态机
*********************************************************************************************************/
  //TODO(成员C):
  //每次捕获到信号时在ISR中重置s_timeoutCnt=0
  //此处累加s_timeoutCnt，超过CAPTURE_NO_SIGNAL_TIMEOUT_MS则清零频率、置无信号
void  Capture_ProcessTimeout(void)
{
  if(s_hasSignal)
  {
    s_timeoutCnt++;
    if(s_timeoutCnt >= CAPTURE_NO_SIGNAL_TIMEOUT_MS)
    {
      s_freqValue    = 0;
      s_captureDuty  = 0;
      s_hasSignal    = 0;
      s_timeoutCnt   = 0;
      s_lastRisingTs = 0;
      s_capState     = CAP_WAIT_RISING;
    }
  }
}

/*********************************************************************************************************
* 函数名称：TIM1_CC_IRQHandler
* 函数功能：TIM1捕获中断，双边沿状态机，计算频率+占空比
* 输入参数：void
* 输出参数：void
* 返 回 值：void
* 创建日期：2026年08月
* 注    意：每次捕获合并溢出计数生成绝对时间戳；自动切换上升/下降沿
*********************************************************************************************************/
void TIM1_CC_IRQHandler(void)
{
  u32 curTs;
  static u32 lastHighTick = 0; //保存上一个脉冲高电平时长
  u32 periodTick;

  if(TIM_GetITStatus(TIM1, TIM_IT_CC1) == SET)
  {
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
    curTs = TIM_GetCapture1(TIM1) + s_overflowCnt * 65536UL;
    s_overflowCnt = 0;

    switch(s_capState)
    {
      case CAP_WAIT_RISING:
      {
        s_curRisingTs = curTs;
        TIM1->CCER |= TIM_CCER_CC1P;
        s_capState = CAP_WAIT_FALLING;

        // 有前一次上升沿，计算周期、频率、占空比
        if(s_lastRisingTs != 0)
        {
          periodTick = s_curRisingTs - s_lastRisingTs;
          if(periodTick != 0)
          {
            u32 tempFreq = 1000000UL / periodTick;                    //频率计算
            if(tempFreq >= CAPTURE_FREQ_MIN && tempFreq <= CAPTURE_FREQ_MAX)
            {
              s_freqValue = tempFreq;
              s_captureDuty = (u8)((lastHighTick * 100UL) / periodTick);
            }
            else
            {
              s_freqValue = 0;
              s_captureDuty = 0;
            }
          }
          s_hasSignal = 1;
          s_timeoutCnt = 0;
        }
        break;
      }

      case CAP_WAIT_FALLING:
      {
        s_fallingTs = curTs;
        lastHighTick = s_fallingTs - s_curRisingTs;

        TIM1->CCER &= ~TIM_CCER_CC1P;
        s_capState = CAP_WAIT_RISING;
        s_lastRisingTs = s_curRisingTs;
        break;
      }
      default: break;
    }
  }
}

/*********************************************************************************************************
* 函数名称：TIM1_UP_IRQHandler
* 函数功能：TIM1更新溢出中断，统计定时器溢出次数
* 输入参数：void
* 输出参数：void
* 返 回 值：void
* 创建日期：2026年08月
*********************************************************************************************************/
void TIM1_UP_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
  {
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    s_overflowCnt++;
  }
}
