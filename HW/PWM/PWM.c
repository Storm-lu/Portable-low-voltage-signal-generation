/*********************************************************************************************************
* 模块名称：PWM.c
* 摘    要：PWM数字脉冲输出模块实现，TIM3 CH2(PB5)输出PWM、频率/占空比切换、启停控制
* 当前版本：1.0.0
* 作    者：成员B（信号发生模块）
* 完成日期：2026年08月
* 内    容：基础任务5——PWM数字脉冲发生
*           频率档位：100Hz / 1kHz / 5kHz
*           占空比档位：25% / 50% / 75%
* 注    意：固定PSC=71，计数速率1MHz；频率 = 1000000/(ARR+1)；使用TIM3部分重映射，CH2输出PB5
*           PWM模式2，输出极性Low；开启预装载；频率误差须 ≤ 2%
**********************************************************************************************************
* 取代版本：TIM3 CH1(PA6)版本
* 修改内容：硬件迁移TIM3 CH2 PB5；对齐项目金标准函数名与逻辑；采用ARR查表、千分比计算CCR
*********************************************************************************************************/

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "PWM.h"
#include "stm32f10x_conf.h"

/*********************************************************************************************************
*                                              内部常量与变量
*********************************************************************************************************/
/*
 * 频率查找表：每种 PWMFreq 选项对应的 ARR 值。
 * 计数速率 = 72MHz / (PSC+1) = 72MHz / 72 = 1MHz
 * PWM 频率 = 1MHz / (ARR+1)
 *
 * 索引：  PWM_FREQ_100Hz -> 9999  -> 1MHz/10000  = 100Hz
 *         PWM_FREQ_1KHz  -> 999   -> 1MHz/1000   = 1kHz
 *         PWM_FREQ_5KHz  -> 199   -> 1MHz/200    = 5kHz
 */
static const u16 s_freqArr[] = {9999, 999, 199};

/*
 * 占空比查找表：每种 PWMDuty 选项对应的千分比（0~1000）。
 * CCR = (ARR+1) * duty_permille / 1000
 *
 * 索引：  PWM_DUTY_25 -> 250  -> 25%
 *         PWM_DUTY_50 -> 500  -> 50%
 *         PWM_DUTY_75 -> 750  -> 75%
 */
static const u16 s_dutyArr[] = {250, 500, 750};

static PWMFreq s_freq = PWM_FREQ_1KHz;
static PWMDuty s_duty = PWM_DUTY_50;
static u8 s_pwmRunning = 0;   //PWM运行标志

/*********************************************************************************************************
*                                              内部函数原型
*********************************************************************************************************/
static void ConfigTimer3ForPWMPB5(u16 arr, u16 psc);

/*********************************************************************************************************
*                                              内部函数实现
*********************************************************************************************************/
/*
 * 配置 TIM3 通道 2，在 PB5 上输出 PWM。
 *
 * 必要设置：
 *   - 使能 TIM3、GPIOB、AFIO 时钟
 *   - GPIO_PartialRemap_TIM3：将 TIM3_CH2 从 PA7 重映射到 PB5
 *   - PB5：GPIO_Mode_AF_PP（复用功能推挽输出）
 *   - TIM3：向上计数，PSC=71（1MHz 计数速率）
 *   - TIM3 CH2：PWM 模式 2，输出使能，极性 Low
 *   - 使能 TIM3 预装载，用于 OC2
 *
 * 参数 arr：自动重装载值（决定频率）
 * 参数 psc：预分频器值（通常为 71，得到 1MHz 计数）
 */
static void ConfigTimer3ForPWMPB5(u16 arr, u16 psc)
{
    GPIO_InitTypeDef        GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef       TIM_OCInitStructure;

    //开启时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    //TIM3部分重映射：TIM3_CH2 -> PB5
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);

    //PB5 复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //时基配置
    TIM_TimeBaseStructure.TIM_Prescaler     = psc;
    TIM_TimeBaseStructure.TIM_Period        = arr;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    //CH2 PWM模式2，极性低
    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_Low;
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);

    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM3, ENABLE);

    TIM_SetCompare2(TIM3, 0); //初始CCR=0，无输出
    TIM_Cmd(TIM3, DISABLE);
}

/*********************************************************************************************************
*                                              API函数实现
*********************************************************************************************************/
/*
 * 使用默认设置（1kHz，50% 占空比）初始化 PWM。
 * 配置定时器并将初始 CCR 设为 0（无输出）。
 */
void InitPWM(void)
{
    s_freq = PWM_FREQ_1KHz;
    s_duty = PWM_DUTY_50;
    s_pwmRunning = 0;
    ConfigTimer3ForPWMPB5(s_freqArr[s_freq], 71);
}

/*
 * 以指定的频率和占空比启动 PWM 输出。
 *
 * 步骤：
 *   1. 将频率和占空比保存到内部状态
 *   2. 使用 s_freqArr[freq] 和 PSC=71 配置 TIM3
 *   3. 计算 CCR = (ARR+1) * s_dutyArr[duty] / 1000
 *   4. 通过 TIM_SetCompare2 设置 CCR
 *   5. 使能 TIM3
 *
 * 参数 freq：PWMFreq 枚举值（100Hz / 1kHz / 5kHz）
 * 参数 duty：PWMDuty 枚举值（25% / 50% / 75%）
 */
void StartPWM(PWMFreq freq, PWMDuty duty)
{
    u16 arrVal;
    u32 ccrVal;

    s_freq = freq;
    s_duty = duty;
    arrVal = s_freqArr[s_freq];

    //更新ARR
    TIM_SetAutoreload(TIM3, arrVal);
    //计算CCR
    ccrVal = (u32)(arrVal + 1) * s_dutyArr[s_duty] / 1000UL;
    TIM_SetCompare2(TIM3, (u16)ccrVal);

    TIM_Cmd(TIM3, ENABLE);
    s_pwmRunning = 1;
}

/*
 * 停止 PWM 输出。
 * 将 CCR 设为 0 并禁用 TIM3。
 */
void StopPWM(void)
{
    TIM_SetCompare2(TIM3, 0);
    TIM_Cmd(TIM3, DISABLE);
    s_pwmRunning = 0;
}

/*
 * 不重启定时器，修改 PWM 频率。
 * 使用 TIM_SetAutoreload 修改 ARR，然后根据当前占空比重算 CCR。
 *
 * 参数 freq：新的 PWMFreq 枚举值
 */
void SetPWMFreq(PWMFreq freq)
{
    u16 arrVal;
    u32 ccrVal;

    s_freq = freq;
    arrVal = s_freqArr[s_freq];
    TIM_SetAutoreload(TIM3, arrVal);

    ccrVal = (u32)(arrVal + 1) * s_dutyArr[s_duty] / 1000UL;
    TIM_SetCompare2(TIM3, (u16)ccrVal);
}

/*
 * 不重启定时器，修改 PWM 占空比。
 * 根据当前 ARR 重新计算 CCR = (ARR+1) * s_dutyArr[duty] / 1000。
 *
 * 参数 duty：新的 PWMDuty 枚举值
 */
void SetPWMDuty(PWMDuty duty)
{
    u16 arrVal;
    u32 ccrVal;

    s_duty = duty;
    arrVal = s_freqArr[s_freq];
    ccrVal = (u32)(arrVal + 1) * s_dutyArr[s_duty] / 1000UL;
    TIM_SetCompare2(TIM3, (u16)ccrVal);
}
