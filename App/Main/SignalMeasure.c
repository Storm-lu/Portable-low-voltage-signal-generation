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
void DisplaySignalMeasure(void){
    /* 待实现：按上述说明实现。
     *
     * 关键点：
     *   - V: 显示用 maxV（不是 avg，显示峰值而非平均值）
     *   - 波形自动缩放（gain=0），使任意幅度信号都能填满显示区域
     *   - 右列起始 x=80，每行间隔 16 像素
     *   - 数字用 OLEDShowNum，标签/单位用 OLEDShowString */
}
