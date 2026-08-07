#ifndef _ADC_H_
#define _ADC_H_


#include "DataType.h"


/*
 * ADC 信号测量模块。
 * 使用 ADC1 通道 1（PA1）+ DMA1 通道 1 进行连续采样。
 *
 * 硬件：PA1（模拟输入）、ADC1、DMA1 通道 1
 * ADC 时钟：12MHz，采样时间：239.5 周期 -> 约 47.6kHz 采样率
 * 缓冲区：DMA 循环模式下的 128 个采样值
 *
 * 电压范围：0~3.3V -> ADC 值 0~4095（12 位）
 * 警告：输入电压不得为负或超过 3.3V（会损坏芯片）
 */


/* DMA 循环缓冲区中的采样点数量 */
#define ADC_BUF_SIZE 128

/*
 * 初始化 ADC1 + DMA1 通道 1。
 * 配置 PA1 为模拟输入，启动连续转换 + DMA。
 * 在 InitHardware() 中调用一次。
 */
void InitADC(void);

/*
 * 返回指向 128 个采样值的 ADC 缓冲区（u16 数组）的指针。
 * 缓冲区由 DMA 在循环模式下持续更新。
 * 注意：读取时数据可能部分更新（DMA 仍在运行中）。
 *
 * 返回值：指向 s_arrADCSamples[ADC_BUF_SIZE] 的指针
 *
 * 调用者：SignalMeasure.c -> DisplaySignalMeasure()
 */
u16* GetADCBuf(void);

/*
 * 返回最新的单个 ADC 采样值。
 * 通过 DMA 剩余计数器定位最新写入位置。
 *
 * 返回值：u16 ADC 值（0~4095）
 *
 * 调用者：（可选，用于单次采样轮询）
 */
u16 GetADCLatest(void);

/*
 * 计算整个 ADC 缓冲区（128 个采样值）的统计信息。
 *
 * 参数 pAvg：输出平均值（0~4095）
 * 参数 pMax：输出最大值（0~4095）
 * 参数 pMin：输出最小值（0~4095）
 * 参数 pP2P：输出峰峰值 = 最大值 - 最小值（0~4095）
 *
 * 电压转换：mV = 值 * 3300 / 4095
 *
 * 调用者：SignalMeasure.c、Main.c -> Proc1SecTask()
 */
void GetADCStats(u16 *pAvg, u16 *pMax, u16 *pMin, u16 *pP2P);

#endif
