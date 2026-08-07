#ifndef __SIGNALOUTPUT__H
#define __SIGNALOUTPUT__H

/*
 * 信号输出模式。
 * 顺序很重要：DC_1V=0, DC_2V=1, TRIANGLE=2, PWM=3, SINE=4
 * 设置页面用 KEY2 在这些模式间循环切换。
 */
typedef enum{
    DC_1V,
    DC_2V,
    TRIANGLE,
    PWM,
    SINE
} SignalOutputType;

/*
 * 初始化信号输出模块。
 * 从 Settings 读取已保存的模式（Flash），将输出设为停止。
 */
void InitSignalOutput(void);

/*
 * 在 OLED 上显示信号输出页面。
 * 显示内容：模式名称、运行/停止状态、按键提示。
 */
void DisplaySignalOutput(void);

/*
 * 切换输出启动/停止（信号输出页面的 KEY1 动作）。
 * 根据当前模式启动 DAC/PWM，或停止全部输出。
 */
void OutputChange(void);

/*
 * 切换信号输出类型并立即重启输出。
 * 在设置页面用户修改模式后由 SaveSettings() 调用。
 *
 * 对于 TRIANGLE/SINE：调用 SetDACWaveTri/Sine + StartDACWave。
 * 对于 PWM：用 Settings 参数调用 StartPWM。
 * 对于 DC：用校准的电压值调用 StartDAC。
 */
void SetSignalOutputType(SignalOutputType type);

/*
 * 获取当前信号输出类型。
 * 供 Proc1SecTask（USART 输出）和 Settings.c 使用。
 */
SignalOutputType GetSignalOutputType(void);

#endif
