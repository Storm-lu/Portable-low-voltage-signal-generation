/*********************************************************************************************************
* 模块名称：Capture.c
* 摘    要：输入捕获频率测量模块，基于TIM1 CH1(PA8)，上升沿+下降沿双捕获测量脉冲频率+占空比
* 当前版本：1.2.0
* 作    者：成员C（信号测量模块）
* 完成日期：2026年08月
* 内    容：基础任务8——数字脉冲频率测量
*           进阶：上升沿+下降沿双捕获实现占空比测量、8周期滑动平均滤波
* 注    意：定时器分频PSC=71，计数频率1MHz；测量误差须 ≤ 2%
*           修复：加入定时器溢出计数，解决16位计数器跨溢出测量异常问题
**********************************************************************************************************
* 取代版本：TIM4 PB6 / InputCapture.c
* 修改内容：变量命名对齐金标准；函数名统一；增加滑动平均；规范返回0xFF；NVIC优先级调整；PA8改为上拉输入；超时改为2ms调用
*********************************************************************************************************/

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "Capture.h"
#include "stm32f10x_conf.h"

/*
 * 频率测量结果（Hz）。0 = 无信号。
 * 由 TIM1_CC_IRQHandler 更新，由 GetCaptureFreq() 读取。
 */
static volatile u32 s_freq      = 0;

/*
 * 占空比测量结果（0~100）。0xFF = 无效/无信号。
 * 由 TIM1_CC_IRQHandler 更新，由 GetCaptureDuty() 读取。
 */
static volatile u8  s_duty      = 0xFF;

/*
 * 边沿跟踪：0 = 等待上升沿，1 = 等待下降沿。
 * 在上升沿和下降沿之间交替切换，以测量周期和占空比。
 */
static volatile u8  s_edgeMode  = 0;

/*
 * TIM1 溢出计数器。每次溢出 = 65536 个计数 = 在 1MHz 下为 65.536ms。
 * 用于测量超过一个定时器周期的长周期。
 */
static volatile u16 s_ovfCnt    = 0;

/*
 * 上一次捕获的 CCR 值（上一次边沿时的定时器计数值）。
 * 用于计算两次边沿之间的时间差。
 */
static volatile u16 s_lastCCR   = 0;

/*
 * 上一次测得的完整周期（以定时器计数为单位 = 在 1MHz 下为微秒）。
 * 在上升沿时保存，下降沿时用于计算占空比。
 */
static volatile u32 s_lastPeriod = 0;

/*
 * 无信号计数器，由 CaptureTimeoutCheck() 每 2ms 递增一次。
 * 若达到 500（1 秒），则频率/占空比重置为"无信号"。
 */
static volatile u16 s_noSignalCnt = 0;

/*
 * 多周期平均缓冲区（高级功能）。
 * 存储最近的频率测量值用于取平均。
 */
#define AVG_COUNT 8
static volatile u32 s_freqBuf[AVG_COUNT];
static volatile u8  s_freqIdx = 0;
static volatile u8  s_freqValid = 0;

/*
 * 配置 TIM1 通道 1 输入捕获，使用 PA8。
 *
 * 必要设置：
 *   - PA8：GPIO_Mode_IPU（输入上拉，用于数字信号输入）
 *   - TIM1：PSC=71（72MHz/72 = 1MHz，1 个计数 = 1us），ARR=65535
 *   - TIM1 CH1：输入捕获，上升沿，直接 TI，无预分频器，无滤波器
 *   - 使能 TIM_IT_CC1（捕获中断）和 TIM_IT_Update（溢出中断）
 *   - NVIC：TIM1_CC_IRQn（优先级 0/0），TIM1_UP_IRQn（优先级 0/1）
 *   - 使能 TIM1
 *
 * 测量范围：约 15Hz（65.5ms 周期）至约 1MHz（受中断延迟限制）
 */
static void ConfigCapture(void)
{
    GPIO_InitTypeDef        GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef       TIM_ICInitStructure;
    NVIC_InitTypeDef        NVIC_InitStructure;

    //开启时钟 TIM1、GPIOA APB2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA, ENABLE);

    //PA8 上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //TIM1时基
    TIM_TimeBaseStructure.TIM_Period        = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler     = 71;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    //CH1 上升沿捕获初始化
    TIM_ICInitStructure.TIM_Channel     = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter    = 0x00;
    TIM_ICInit(TIM1, &TIM_ICInitStructure);

    //NVIC TIM1 CC中断 抢占0，子优先级0
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //NVIC TIM1 更新溢出中断 抢占0，子优先级1
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ITConfig(TIM1, TIM_IT_CC1 | TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM1, ENABLE);
}

/*
 * 初始化捕获系统。
 * 调用 ConfigCapture()。
 */
void InitCapture(void)
{
    s_freq = 0;
    s_duty = 0xFF;
    s_edgeMode = 0;
    s_ovfCnt = 0;
    s_lastCCR = 0;
    s_lastPeriod = 0;
    s_noSignalCnt = 0;
    s_freqIdx = 0;
    s_freqValid = 0;

    ConfigCapture();
}

/*
 * 返回当前测得的频率（Hz）。
 * 若检测到无信号（超时），返回 0。
 *
 * 调用者：SignalMeasure.c、Main.c -> Proc1SecTask()
 */
u32 GetCaptureFreq(void)
{
    return s_freq;
}

/*
 * 返回当前测得的占空比（0~100）。
 * 若无信号或测量无效，返回 0xFF。
 *
 * 调用者：SignalMeasure.c、Main.c -> Proc1SecTask()
 */
u8 GetCaptureDuty(void)
{
    return s_duty;
}

/*
 * 周期性超时检测，由 Proc2msTask() 每 2ms 调用一次。
 *
 * 若 1 秒内（500 x 2ms）无捕获事件发生：
 *   - 将 s_freq 设为 0（无信号）
 *   - 将 s_duty 设为 0xFF（无效）
 *
 * 捕获 ISR 在每次有效边沿时将 s_noSignalCnt 重置为 0。
 */
void CaptureTimeoutCheck(void)
{
    s_noSignalCnt++;
    if(s_noSignalCnt >= 500U)
    {
        s_freq = 0;
        s_duty = 0xFF;
        s_noSignalCnt = 0;
    }
}

/*
 * TIM1 溢出中断处理函数。
 *
 * 当 TIM1 计数器从 65535 回绕到 0 时调用。
 * 递增 s_ovfCnt 以跟踪长周期的总经过时间。
 *
 * 必须清除 TIM_IT_Update 挂起位。
 */
void TIM1_UP_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        s_ovfCnt++;
    }
}

/*
 * TIM1 捕获/比较中断处理函数。
 *
 * 这是核心测量算法：
 *
 * 上升沿时（s_edgeMode == 0）：
 *   1. 计算总计数 = s_ovfCnt * 65536 + curCCR - s_lastCCR
 *   2. 周期（us）= 总计数（因为 1MHz 计数速率）
 *   3. 频率 = 1000000 / 周期
 *   4. 存入平均缓冲区，计算平均值 -> s_freq
 *   5. 保存 s_lastPeriod 供下降沿计算使用
 *   6. 保存 s_lastCCR = curCCR，重置 s_ovfCnt = 0
 *   7. 切换到下降沿：TIM1->CCER |= TIM_CCER_CC1P
 *   8. 设置 s_edgeMode = 1
 *
 * 下降沿时（s_edgeMode == 1）：
 *   1. 计算高电平时间 = s_ovfCnt * 65536 + curCCR - s_lastCCR
 *   2. 占空比 = 高电平时间 * 100 / s_lastPeriod
 *   3. 将占空比限制在 0~100 范围内，存入 s_duty
 *   4. 切换回上升沿：TIM1->CCER &= ~TIM_CCER_CC1P
 *   5. 设置 s_edgeMode = 0
 *
 * 任意边沿时：
 *   - 清除 TIM_IT_CC1 挂起位
 *   - 重置 s_noSignalCnt = 0（信号存在）
 *
 * 注意：标准库中不存在 TIM_SetIC1Polarity。
 *       请直接操作 TIM1->CCER 寄存器：
 *         上升沿：  TIM1->CCER &= ~TIM_CCER_CC1P
 *         下降沿：TIM1->CCER |= TIM_CCER_CC1P
 */
void TIM1_CC_IRQHandler(void)
{
    u16 curCCR;
    u32 deltaTick;
    u32 highTick;
    u32 freqRaw;
    u32 freqSum = 0;
    u8 i;

    if(TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
        curCCR = TIM_GetCapture1(TIM1);
        s_noSignalCnt = 0; //收到信号，清除超时计数

        if(s_edgeMode == 0)
        {
            //等待上升沿
            deltaTick = (u32)s_ovfCnt * 65536UL + curCCR - s_lastCCR;
            s_ovfCnt = 0;

            if(deltaTick > 0)
            {
                freqRaw = 1000000UL / deltaTick;
                //存入滑动平均缓冲区
                s_freqBuf[s_freqIdx] = freqRaw;
                s_freqIdx = (s_freqIdx + 1) % AVG_COUNT;
                if(s_freqValid < AVG_COUNT)
                {
                    s_freqValid++;
                }
                //求取平均值
                for(i = 0; i < s_freqValid; i++)
                {
                    freqSum += s_freqBuf[i];
                }
                s_freq = freqSum / s_freqValid;
                s_lastPeriod = deltaTick;
            }

            s_lastCCR = curCCR;
            //切换捕获极性：下降沿
            TIM1->CCER |= TIM_CCER_CC1P;
            s_edgeMode = 1;
        }
        else
        {
            //等待下降沿
            highTick = (u32)s_ovfCnt * 65536UL + curCCR - s_lastCCR;
            s_ovfCnt = 0;

            if(s_lastPeriod != 0)
            {
                u32 dutyTemp = (highTick * 100UL) / s_lastPeriod;
                if(dutyTemp > 100) dutyTemp = 100;
                s_duty = (u8)dutyTemp;
            }
            //切换捕获极性：上升沿
            TIM1->CCER &= ~TIM_CCER_CC1P;
            s_edgeMode = 0;
            s_lastCCR = curCCR;
        }
    }
}
