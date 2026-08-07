#include "PWM.h"
#include "stm32f10x_conf.h"


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


/* ---- 内部函数原型 ---- */
static void ConfigTimer3ForPWMPB5(u16 arr, u16 psc);


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
    /* 待实现：在 PB5 上配置 TIM3 CH2 PWM 输出 */
}


/*
 * 使用默认设置（1kHz，50% 占空比）初始化 PWM。
 * 配置定时器并将初始 CCR 设为 0（无输出）。
 */
void InitPWM(void)
{
    /* 待实现：初始化 PWM 系统 */
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
 *
 * 调用者：SignalOutput.c -> OutputChange()、SetSignalOutputType()
 */
void StartPWM(PWMFreq freq, PWMDuty duty)
{
    /* 待实现：以给定参数启动 PWM */
}


/*
 * 停止 PWM 输出。
 * 将 CCR 设为 0 并禁用 TIM3。
 *
 * 调用者：SignalOutput.c、Main.c
 */
void StopPWM(void)
{
    /* 待实现：停止 PWM 输出 */
}


/*
 * 不重启定时器，修改 PWM 频率。
 * 使用 TIM_SetAutoreload 修改 ARR，然后根据当前占空比重算 CCR。
 *
 * 参数 freq：新的 PWMFreq 枚举值
 *
 * 调用者：Settings.c -> SaveSettings()
 */
void SetPWMFreq(PWMFreq freq)
{
    /* 待实现：运行中修改 PWM 频率 */
}


/*
 * 不重启定时器，修改 PWM 占空比。
 * 根据当前 ARR 重新计算 CCR = (ARR+1) * s_dutyArr[duty] / 1000。
 *
 * 参数 duty：新的 PWMDuty 枚举值
 *
 * 调用者：Settings.c -> SaveSettings()
 */
void SetPWMDuty(PWMDuty duty)
{
    /* 待实现：运行中修改 PWM 占空比 */
}
