#include "Main.h"
#include "stm32f10x_conf.h"
#include "DataType.h"
#include "SignalOutput.h"
#include "DAC.h"
#include "PWM.h"
#include "Settings.h"
#include "OLED.h"

static SignalOutputType signal_mode;
static u8 signal_state = 0;

/*
 * 初始化信号输出模块。
 *
 * 从 Settings 读取已保存的模式（Flash 加载）：
 *   signal_mode = GetSettingsSignalMode();
 *   signal_state = 0;  （启动时输出停止）
 */
void InitSignalOutput(void){
    /* 待实现：从 Settings 读取模式，状态设为停止。 */
    signal_mode = GetSettingsSignalMode();
    signal_state = 0;
}

/*
 * 在 OLED 上显示信号输出页面。
 *
 * 布局：
 *   第 0 行 (y=0)：  "Signal Output:"
 *   第 1 行 (y=16)： "M:" + 模式名称
 *                   DC_1V -> "DC 1.0V", DC_2V -> "DC 2.0V"
 *                   TRIANGLE -> "TRI 100Hz", SINE -> "SINE 100Hz"
 *                   PWM -> "PWM"
 *   第 2 行 (y=32)： "S:" + "RUNNING" 或 "STOPPED"（取决于 signal_state）
 *   第 3 行 (y=48)： "KEY1: Start/Stop"
 *
 * 注意：开头调用 OLEDClear()，结尾调用 OLEDRefreshGRAM()。
 */
void DisplaySignalOutput(void){
    /* 待实现：按上述说明实现显示布局。 */
    OLEDClear();

    //标题
    OLEDShowString(0, 0, (const u8*)"Signal Output:");
    OLEDShowString(0, 16, (const u8*)"M:");
    //显示模式名称
    switch (signal_mode)
    {
    case DC_1V:
        OLEDShowString(16, 16, (const u8*)"DC 1.0V");
        break;
    case DC_2V:
        OLEDShowString(16, 16, (const u8*)"DC 2.0V");
        break;
    case TRIANGLE:
        OLEDShowString(16, 16, (const u8*)"TRI 100Hz");
        break;
    case SINE:
        OLEDShowString(16, 16, (const u8*)"SINE 100Hz");
        break;
    case PWM:
        OLEDShowString(16, 16, (const u8*)"PWM");
        break;
    default:
        OLEDShowString(16, 16, (const u8*)"DC 1.0V");
        break;
    }
    //显示状态
    OLEDShowString(0, 32, (const u8*)"S:");
    if(signal_state)
    {
        OLEDShowString(16, 32, (const u8*)"RUNNING");
    }
    else
    {
        OLEDShowString(16, 32, (const u8*)"STOPPED");
    }

    //显示提示
    OLEDShowString(0, 48, (const u8*)"KEY1: Start/Stop");
    OLEDRefreshGRAM();//刷新显示
}

/*
 * 切换输出启动/停止（信号输出页面的 KEY1 动作）。
 *
 * 若当前正在运行（signal_state==1）：
 *   - 停止：StopDAC(), StopPWM(), signal_state=0
 *
 * 若当前已停止（signal_state==0）：
 *   - 根据模式启动：
 *     TRIANGLE/SINE -> StartDACWave()
 *     PWM            -> StartPWM(GetSettingsPWMFreq(), GetSettingsPWMDuty())
 *     DC_1V          -> StartDAC(1241)   (1241/4095 * 3.3V ~ 1.0V)
 *     DC_2V          -> StartDAC(2482)   (2482/4095 * 3.3V ~ 2.0V)
 *   - signal_state=1
 */
void OutputChange(void){
    /* 待实现：按上述说明实现启动/停止切换。 */
    if (signal_state)
    {
        StopDAC();
        StopPWM();
        signal_state = 0;
    }
    else
    {
        switch (signal_mode)
        {
        case TRIANGLE:
            SetDACWaveTri();
            StartDACWave();
            break;
        case SINE:
            SetDACWaveSine();
            StartDACWave();
            break;
        case PWM:
            StartPWM(GetSettingsPWMFreq(), GetSettingsPWMDuty());
            break;
        case DC_1V:
            StartDAC(1241);
            break;
        case DC_2V:
            StartDAC(2482);
            break;
        default:
            StartDAC(1241);
            break;
        }
        signal_state = 1;
    }
    
}

/*
 * 切换信号输出类型并重启输出（由设置保存时调用）。
 *
 * 步骤：
 *   1. 若当前正在运行：停止全部（StopDAC, StopPWM），置状态=0
 *   2. 设 signal_mode = type
 *   3. 启动新模式：
 *      TRIANGLE -> SetDACWaveTri() + StartDACWave()
 *      SINE     -> SetDACWaveSine() + StartDACWave()
 *      PWM      -> StartPWM(GetSettingsPWMFreq(), GetSettingsPWMDuty())
 *      DC_1V    -> StartDAC(1241)
 *      DC_2V    -> StartDAC(2482)
 *   4. signal_state = 1
 *
 * 注意：对于 TRIANGLE/SINE，必须同时调用 SetDACWave* 和 StartDACWave。
 *       SetDACWaveTri/Sine 仅设置缓冲区指针，StartDACWave 才启动 DMA。
 */
void SetSignalOutputType(SignalOutputType type){
    /* 待实现：按上述说明实现模式切换 + 重启。 */
    if (signal_state)
    {
        StopDAC();
        StopPWM();
        signal_state = 0;
    }
    signal_mode = type;//设置新的信号输出类型
    //根据新的信号输出类型启动相应的输出
    switch (signal_mode)
    {
    case TRIANGLE:
        SetDACWaveTri();
        StartDACWave();
        break;
    case SINE:
        SetDACWaveSine();
        StartDACWave();
        break;
    case PWM:
        StartPWM(GetSettingsPWMFreq(), GetSettingsPWMDuty());
        break;
    case DC_1V:
        StartDAC(1241);
        break;
    case DC_2V:
        StartDAC(2482);
        break;
    default:
        StartDAC(1241);
        break;
    }

    //设置 signal_state 为 1，表示输出已启动
    signal_state = 1;

}

/*
 * 获取当前信号输出类型。
 * 供 Proc1SecTask 用于 USART 输出，以及供 Settings.c 使用。
 */
SignalOutputType GetSignalOutputType(void){
    return signal_mode;
}
