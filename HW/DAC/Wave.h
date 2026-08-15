#ifndef _WAVE_H_
#define _WAVE_H_


#include "DataType.h"


/*
 * 波形数据生成模块。
 * 提供供 DAC 波形输出使用的 100 点查找表。
 * 所有波形：100 个采样点，u16 值，范围 0~4095（12 位 DAC）。
 *
 * 波形输出频率 = DAC 采样率 / 100
 *   例如：10kHz 采样率 -> 100Hz 输出频率
 */


/*
 * 初始化波形数据。
 * 如果使用编译时查找表，可为空函数。
 */
void InitWave(void);

/*
 * 返回指向 100 点正弦波数据数组的指针。
 * 值范围：0~4095，以 2048 为中心。
 *
 * 调用者：DAC.c -> InitDAC()、SetDACWaveSine()
 */
u16* GetSineWave100PointAddr(void);

/* [FIX] 已删除 GetRectWave100PointAddr() 声明 —— Wave.c 中已移除该函数实现，
 *       保留声明会导致潜在的链接错误。 */

/*
 * 返回指向 100 点三角波数据数组的指针。
 * 值范围：0~4095，线性先上升后下降。
 *
 * 调用者：DAC.c -> InitDAC()、SetDACWaveTri()
 */
u16* GetTriWave100PointAddr(void);

#endif
