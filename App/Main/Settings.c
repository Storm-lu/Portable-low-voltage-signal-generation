#include "Main.h"
#include "stm32f10x_conf.h"
#include "DataType.h"
#include "Settings.h"
#include "PWM.h"
#include "SignalOutput.h"
#include "Flash.h"
#include "OLED.h"

#define FLASH_SETTINGS_ADDR  0x0803F800UL
#define FLASH_MAGIC_NUMBER   0x12345678UL

static SignalOutputType signal_mode = DC_1V;
static PWMFreq pwm_freq = PWM_FREQ_1KHz;
static PWMDuty pwm_duty = PWM_DUTY_50;
static SettingItem current_setting = SETTING_SIGNAL_OUTPUT;

/*
 * 从 Flash 初始化设置。
 *
 * 步骤：
 *   1. 通过 STM32FlashReadWord 从 FLASH_SETTINGS_ADDR 读取 4 个字
 *   2. 检查 buf[0] == FLASH_MAGIC_NUMBER（存在有效数据）
 *      若是：从 buf[1..3] 加载 signal_mode、pwm_freq、pwm_duty
 *            校验每个枚举值（防止 Flash 损坏）：
 *              signal_mode > SINE -> 复位为 DC_1V
 *              pwm_freq > PWM_FREQ_5KHz -> 复位为 PWM_FREQ_100Hz
 *              pwm_duty > PWM_DUTY_75 -> 复位为 PWM_DUTY_25
 *      若否：使用默认值（DC_1V, PWM_FREQ_1KHz, PWM_DUTY_50）
 *   3. 设 current_setting = SETTING_SIGNAL_OUTPUT
 *
 * 必须在 InitSignalOutput() 之前调用（后者会读取 GetSettingsSignalMode）。
 */
void InitSettings(void){
    /* 待实现：从 Flash 读取、校验、加载或使用默认值。 */
    u32 buf[4];

    STM32FlashReadWord(FLASH_SETTINGS_ADDR, buf, 4);

    if(buf[0] == FLASH_MAGIC_NUMBER)
    {

        signal_mode = (SignalOutputType)buf[1];
        pwm_freq = (PWMFreq)buf[2];
        pwm_duty = (PWMDuty)buf[3];

        //如果 signal_mode 超过最大值，则复位为 DC_1V
        if(signal_mode > SINE)
        {
            signal_mode = DC_1V;
        }

        //如果 pwm_freq 超过最大值，则复位为 PWM_FREQ_100Hz
        if(pwm_freq > PWM_FREQ_5KHz)
        {
            pwm_freq = PWM_FREQ_100Hz;
        }

        //如果 pwm_duty 超过最大值，则复位为 PWM_DUTY_25
        if(pwm_duty > PWM_DUTY_75)
        {
            pwm_duty = PWM_DUTY_25;
        }
    }
    //否则使用默认值
    else
    {
        signal_mode = DC_1V;
        pwm_freq = PWM_FREQ_1KHz;
        pwm_duty = PWM_DUTY_50;
    }

    //设置 current_setting 为 SETTING_SIGNAL_OUTPUT
    current_setting = SETTING_SIGNAL_OUTPUT;
}

/*
 * 在 OLED 上显示设置页面。
 *
 * 布局：
 *   y=0：  "Settings"（标题）
 *   y=16： ">" 或 " " + "Mode:" + 模式名称（DC 1V / DC 2V / TRI / PWM / SINE）
 *
 *   以下两行仅在 signal_mode == PWM 时显示：
 *   y=32： ">" 或 " " + "Freq:" + 频率（100Hz / 1kHz / 5kHz）
 *   y=48： ">" 或 " " + "Duty:" + 占空比（25% / 50% / 75%）
 *
 * ">" 光标指示 current_setting（即 KEY2 将修改的项）。
 * 非 PWM 模式：只显示 Mode 行，光标锁定在 Mode。
 *
 * 注意：开头调用 OLEDClear()，结尾调用 OLEDRefreshGRAM()。
 */
void DisplaySettings(void){
    /* 待实现：按上述说明实现布局。 */
    OLEDClear();

    //标题
    OLEDShowString(0,0,(const u8*)"Settings");

    //如果 current_setting == SETTING_SIGNAL_OUTPUT，则显示 ">"，否则显示空格
    if (current_setting == SETTING_SIGNAL_OUTPUT)
    {
        OLEDShowChar(0, 16, '>', 16, 1);
    }
    else
    {
        OLEDShowChar(0, 16, ' ', 16, 1);
    }

    //显示 Mode 行
    OLEDShowString(8, 16, (const u8*)"Mode:");
    switch (signal_mode)
    {
    case DC_1V:
        OLEDShowString(48,16,(const u8*)"DC 1.0V");
        break;
    case DC_2V:
        OLEDShowString(48,16,(const u8*)"DC 2.0V");
        break;
    case TRIANGLE:
        OLEDShowString(48,16,(const u8*)"TRI 100Hz");
        break;
    case PWM:
        OLEDShowString(48,16,(const u8*)"PWM");
        break;
    case SINE:
        OLEDShowString(48,16,(const u8*)"SINE 100Hz");
        break;
    
    default:
        OLEDShowString(48,16,(const u8*)"DC 1.0V");
        break;
    }
    
    //如果 signal_mode == PWM，则显示 Freq 行
    if(signal_mode == PWM)
    {
        if(current_setting == SETTING_PWM_FREQ)//如果 current_setting == SETTING_PWM_FREQ，则显示 ">"，否则显示空格
        {
            OLEDShowChar(0, 32, '>', 16, 1);
        }
        else
        {
            OLEDShowChar(0, 32, ' ', 16, 1);
        }
        OLEDShowString(8, 32, (const u8*)"Freq:");
        //显示频率值
        switch (pwm_freq)
        {
            case PWM_FREQ_100Hz:
                OLEDShowString(48,32,(const u8*)"100Hz");
                break;
            case PWM_FREQ_1KHz:
                OLEDShowString(48,32,(const u8*)"1kHz");
                break;
            case PWM_FREQ_5KHz:
                OLEDShowString(48,32,(const u8*)"5kHz");
                break;
            default:
                OLEDShowString(48,32,(const u8*)"1KHz");
                break;

        }

        if(current_setting == SETTING_PWM_DUTY)
        {
            OLEDShowChar(0, 48, '>', 16, 1);
        }
        else
        {
            OLEDShowChar(0, 48, ' ', 16, 1);
        }

        OLEDShowString(8, 48, (const u8*)"Duty:");
        //显示占空比值
        switch (pwm_duty)
        {
            case PWM_DUTY_25:
                OLEDShowString(48,48,(const u8*)"25%");
                break;
            case PWM_DUTY_50:
                OLEDShowString(48,48,(const u8*)"50%");
                break;
            case PWM_DUTY_75:
                OLEDShowString(48,48,(const u8*)"75%");
                break;
            default:
                OLEDShowString(48,48,(const u8*)"50%");
                break;

        }
    }
    else
    {
        current_setting = SETTING_SIGNAL_OUTPUT; //非 PWM 模式下，current_setting 锁定在 Mode
    }

    OLEDRefreshGRAM();
}

/*
 * 移动光标到下一个设置项（设置页面的 KEY1 动作）。
 *
 * 若 signal_mode == PWM：
 *   循环：SETTING_SIGNAL_OUTPUT -> SETTING_PWM_FREQ -> SETTING_PWM_DUTY -> 回到起点
 * 否则：
 *   光标锁定在 SETTING_SIGNAL_OUTPUT（仅 Mode 可选）
 */
void ParamChange(void){
    /* 待实现：实现光标循环，受 PWM 模式限制。 */
    if(signal_mode == PWM)//如果当前模式为 PWM，则循环切换光标
    {
        if (current_setting == SETTING_SIGNAL_OUTPUT)
        {
            current_setting = SETTING_PWM_FREQ;
        }
        else if(current_setting == SETTING_PWM_FREQ)
        {
            current_setting = SETTING_PWM_DUTY;
        }
        else
        {
            current_setting = SETTING_SIGNAL_OUTPUT;
        }
        
    }
    else//否则光标锁定在 SETTING_SIGNAL_OUTPUT
    {
        current_setting = SETTING_SIGNAL_OUTPUT;
    }

}

/*
 * 递增当前设置值（设置页面的 KEY2 动作）。
 *
 * SETTING_SIGNAL_OUTPUT：循环 signal_mode（DC_1V->DC_2V->TRI->PWM->SINE->DC_1V）
 *   若新模式 != PWM，将 current_setting 复位为 SETTING_SIGNAL_OUTPUT
 *   （因为非 PWM 模式下 Freq/Duty 项不可见）
 * SETTING_PWM_FREQ：循环 pwm_freq（100Hz->1kHz->5kHz->100Hz）
 * SETTING_PWM_DUTY：循环 pwm_duty（25%->50%->75%->25%）
 */
void ValueChange(void){
    /* 待实现：为每个设置项实现值循环。 */
    //进入循环
    if (current_setting == SETTING_SIGNAL_OUTPUT)
    {
        if(signal_mode == SINE)
        {
            signal_mode = DC_1V;
        }
        else
        {
            signal_mode = (SignalOutputType)(signal_mode + 1);//循环切换信号输出模式
        }

        if(signal_mode != PWM)
        {
            current_setting = SETTING_SIGNAL_OUTPUT; //非 PWM 模式下，current_setting 锁定在 Mode
        }
    }
    
    //循环 PWM 频率
    else if(current_setting == SETTING_PWM_FREQ)
    {
        if(pwm_freq == PWM_FREQ_5KHz)
        {
            pwm_freq = PWM_FREQ_100Hz;
        }
        else
        {
            pwm_freq = (PWMFreq)(pwm_freq + 1);//循环切换 PWM 频率
        }
    }
    //循环 PWM 占空比
    else if(current_setting == SETTING_PWM_DUTY)
    {
        if(pwm_duty == PWM_DUTY_75)
        {
            pwm_duty = PWM_DUTY_25;
        }
        else
        {
            pwm_duty = (PWMDuty)(pwm_duty + 1);//循环切换 PWM 占空比
        }
    }
}

/*
 * 保存设置并退出（设置页面的 KEY3 长按 2秒 动作）。
 *
 * 步骤：
 *   1. 应用信号输出：SetSignalOutputType(signal_mode)
 *   2. 若为 PWM 模式：SetPWMFreq(pwm_freq), SetPWMDuty(pwm_duty)
 *   3. 写入 Flash：
 *        buf[0] = FLASH_MAGIC_NUMBER
 *        buf[1] = (u32)signal_mode
 *        buf[2] = (u32)pwm_freq
 *        buf[3] = (u32)pwm_duty
 *        STM32FlashWriteWord(FLASH_SETTINGS_ADDR, buf, 4)
 *   4. 返回上一页面：DisplayChange()
 */
void SaveSettings(void){
    /* 待实现：应用设置、写 Flash、切换页面。 */

    u32 buf[4];

    SetSignalOutputType(signal_mode);//应用信号输出模式

    if(signal_mode == PWM)
    {
        SetPWMFreq(pwm_freq);//应用 PWM 频率
        SetPWMDuty(pwm_duty);//应用 PWM 占空比
    }

    //暂存buf数组，准备写入 Flash
    buf[0] = FLASH_MAGIC_NUMBER;
    buf[1] = (u32)signal_mode;
    buf[2] = (u32)pwm_freq;
    buf[3] = (u32)pwm_duty;

    STM32FlashWriteWord(FLASH_SETTINGS_ADDR, buf, 4);//写入 Flash

    current_setting = SETTING_SIGNAL_OUTPUT; //保存设置后，current_setting 复位为 SETTING_SIGNAL_OUTPUT

    DisplayChange(); //返回上一页面
}

/* ============================================================
 *  Getter 函数 - 已实现，请勿修改。
 *  由 SignalOutput.c 调用以读取已保存的参数。
 * ============================================================ */

SignalOutputType GetSettingsSignalMode(void){
    return signal_mode;
}

PWMFreq GetSettingsPWMFreq(void){
    return pwm_freq;
}

PWMDuty GetSettingsPWMDuty(void){
    return pwm_duty;
}
