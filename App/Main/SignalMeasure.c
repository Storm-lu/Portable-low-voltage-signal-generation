#include "Main.h"
#include "stm32f10x_conf.h"
#include "DataType.h"
#include "SignalMeasure.h"
#include "ADC.h"
#include "Capture.h"
#include "OLED.h"

/* 波形显示区域：左侧宽 80 像素，顶部高 48 像素 */
#define WAVE_X_START    0
#define WAVE_Y_START    0
#define WAVE_WIDTH      80
#define WAVE_HEIGHT     48
/* [FIX] 已删除重复的 #define ADC_BUF_SIZE 128 —— ADC.h 中已有定义，
 *       重复定义可能导致维护不一致。 */
void InitSignalMeasure(void){
    /* 目前无需初始化。 */
}

/*
 * 在 OLED 上显示信号测量页面。
 *
 * 布局（示波器风格）：
 *
 *   +-------- 波形区域 ---------+-- 文本区域 --+
 *   |                          | V:x.xV        |  y=0   （峰值电压）
 *   |  80 x 48 像素带网格       | F:xxxH        |  y=16  （频率）
 *   |  自动缩放波形            | P:x.xV        |  y=32  （峰峰值）
 *   |                          | D:xx%         |  y=48  （占空比）
 *   +--------------------------+---------------+
 *
 * 数据来源：
 *   pADC = GetADCBuf()                      -- 128 点 ADC 缓冲区（A组）
 *   GetADCStats(&avg, &maxV, &minV, &p2p)   -- 统计值（A组）
 *   freq = GetCaptureFreq()                 -- 频率，单位 Hz（B组）
 *   duty = GetCaptureDuty()                 -- 占空比 0~100，0xFF=无效（B组）
 *
 * 电压换算（12 位 ADC，3.3V 参考）：
 *   voltMV = maxV * 3300 / 4095    （峰值电压，单位毫伏）
 *   p2pMV  = p2p  * 3300 / 4095    （峰峰值，单位毫伏）
 *
 * 频率显示格式：
 *   freq == 0        -> "--  "
 *   freq >= 1000     -> "x.xk"（< 10kHz）或 "xxxk"（< 1000kHz）或 ">1M"
 *   freq < 1000      -> "xxxH"
 *
 * 占空比显示：仅当 duty != 0xFF（有效信号）时显示
 *
 * 绘制步骤：
 *   1. OLEDClear()
 *   2. OLEDDrawGrid(WAVE_X_START, WAVE_Y_START, WAVE_WIDTH, WAVE_HEIGHT, 8, 4)
 *   3. OLEDDrawWaveFormEx(pADC, ADC_BUF_SIZE, ..., gain=0)  [自动缩放]
 *   4. 在右列 (x=80) 绘制文本标签和数值
 *   5. OLEDRefreshGRAM()
 */
//显示电压文本
static void ShowVoltageText(u8 x,u8 y,const u8* label, u16 adcValue)
{
    /* 待实现：在 (x, y) 显示 "label:x.xV"。
     *   1. 显示标签（OLEDShowString）
     *   2. 将 valueMV 转换为 x.xV 格式（整数除法）
     *   3. 显示电压值（OLEDShowNum）和单位 'V' */
    u16 mv;
    u16 whole;
    u16 frac;

    mv = (u16)((u32)adcValue * 3300 / 4095); //将 ADC 值转换为毫伏
    whole = (u16)(mv / 1000); //整数部分
    frac = (u16)((mv % 1000) / 100); //小数部分，保留一位小数

    OLEDShowString(x, y, label); //显示标签
    OLEDShowNum((u8)(x + 16), y, whole, 1, 16); //显示整数部分
    OLEDShowChar((u8)(x + 24), y, '.', 16, 1); //显示小数点
    OLEDShowNum((u8)(x + 32), y, frac, 1, 16); //显示小数部分
    OLEDShowChar((u8)(x + 40), y, 'V', 16, 1); //显示单位 'V'
}

//显示频率文本
static void ShowFreqText(u8 x,u8 y, u32 Freq)
{
    if (Freq == 0)
    {
        OLEDShowString(x, y, (const u8*)"F:--  ");
        return;
    }
    if (Freq < 1000)
    {
        OLEDShowString(x, y, (const u8*)"F:");
        OLEDShowNum((u8)(x + 16), y, Freq, 3, 16);  /* [FIX] 4位改3位，避免 'H' 字符越界 */
        OLEDShowChar((u8)(x + 40), y, 'H', 16, 1);  /* [FIX] x+48 越界(80+48=128>127)，改为 x+40 */
    }
    else if(Freq <10000)
    {
        u16 whole = (u16)(Freq / 1000);
        u16 frac = (u16)((Freq % 1000) / 100);
        OLEDShowString(x, y, (const u8*)"F:");
        OLEDShowNum((u8)(x + 16), y, whole, 1, 16);
        OLEDShowChar((u8)(x + 24), y, '.', 16, 1);
        OLEDShowNum((u8)(x + 32), y, frac, 1, 16);
        OLEDShowChar((u8)(x + 40), y, 'k', 16, 1);

    }
    else if(Freq<1000000)
    {
        OLEDShowString(x, y, (const u8*)"F:");
        OLEDShowNum((u8)(x + 16), y, (u16)(Freq / 1000), 3, 16);
        OLEDShowChar((u8)(x + 40), y, 'k', 16, 1);
    }

    else
    {
        OLEDShowString(x, y, (const u8*)"F:>1M");
    }
    
}

//显示占空比文本
static void ShowDutyText(u8 x,u8 y, u8 duty)
{
    if (duty == 0xFF)
    {
        OLEDShowString(x, y, (const u8*)"D:--%");
        return;
    }
    OLEDShowString(x, y, (const u8*)"D:");
    OLEDShowNum((u8)(x + 16), y, duty, 3, 16);
    OLEDShowChar((u8)(x + 40), y, '%', 16, 1);
}

void DisplaySignalMeasure(void){
    /* 待实现：按上述说明实现。
     *
     * 关键点：
     *   - V: 显示用 maxV（不是 avg，显示峰值而非平均值）
     *   - 波形自动缩放（gain=0），使任意幅度信号都能填满显示区域
     *   - 右列起始 x=80，每行间隔 16 像素
     *   - 数字用 OLEDShowNum，标签/单位用 OLEDShowString */

    u16 avg, maxV, minV, p2p;
    u16* pADC;
    u32 freq;
    u8 duty;

    pADC = GetADCBuf(); //获取 ADC 缓冲区
    GetADCStats(&avg, &maxV, &minV, &p2p); //获取 ADC 统计值
    freq = GetCaptureFreq(); //获取频率
    duty = GetCaptureDuty(); //获取占空比

    OLEDClear(); //清屏

    OLEDDrawGrid(WAVE_X_START, WAVE_Y_START, WAVE_WIDTH, WAVE_HEIGHT, 8, 4); //绘制网格
    OLEDDrawWaveFormEx(pADC,ADC_BUF_SIZE , WAVE_X_START, WAVE_Y_START, WAVE_WIDTH, WAVE_HEIGHT, 4095,0); //绘制波形，自动缩放

    ShowVoltageText(80, 0, (const u8*)"V:", maxV); //显示峰值电压
    ShowFreqText(80, 16, freq); //显示频率
    ShowVoltageText(80, 32, (const u8*)"P:", p2p); //显示峰峰值
    ShowDutyText(80, 48, duty); //显示占空比

    OLEDRefreshGRAM(); //刷新显示

}
