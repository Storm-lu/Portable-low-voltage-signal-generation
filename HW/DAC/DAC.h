#ifndef _DAC_H_
#define _DAC_H_


#include "DataType.h"


/*
 * 波形缓冲区描述符。
 * 由 DAC 模块用于配置 DMA 源地址。
 *
 * waveBufAddr：包含波形采样值（0~4095）的 u16 数组地址
 * waveBufSize：数组中的采样点数量（通常为 100）
 */
typedef struct
{
    u32 waveBufAddr;
    u32 waveBufSize;
} StructDACWave;


/*
 * 初始化 DAC 系统。
 * 配置 PA4、DAC 通道 1、TIM4 触发源和 DMA2 通道 3。
 * 默认波形为三角波（100 点，100Hz）。
 * 在 InitHardware() 中调用一次。
 */
void InitDAC(void);

/*
 * 在 PA4 上输出直流电压。
 * 禁用 DMA 波形输出，直接设置 DAC 寄存器。
 *
 * 参数 dacValue：12 位 DAC 值（0~4095）
 *   0V -> 0, 1.0V -> 1241, 2.0V -> 2482, 3.3V -> 4095
 *
 * 调用者：SignalOutput.c -> OutputChange()、SetSignalOutputType()
 */
void StartDAC(u16 dacValue);

/*
 * 在 PA4 上启动 DMA 驱动的波形输出。
 * 使用由 SetDACWaveTri/SetDACWaveSine 设置的波形缓冲区。
 * 波形通过 DMA 循环模式持续循环输出。
 *
 * 调用者：SignalOutput.c -> OutputChange()、SetSignalOutputType()
 */
void StartDACWave(void);

/*
 * 停止所有 DAC 输出（直流和波形）。
 * 禁用 DMA，将 DAC 输出设为 0V。
 *
 * 调用者：SignalOutput.c、Main.c
 */
void StopDAC(void);

/*
 * 设置当前波形缓冲区描述符。
 *
 * 参数 wave：包含缓冲区地址和大小的结构体
 * 调用者：SetDACWaveTri/SetDACWaveSine（内部）、ProcHostCmd.c（外部）
 */
void SetDACWave(StructDACWave wave);

/*
 * 切换波形为三角波（100 点）。
 * 更新内部缓冲区指向三角波数据。
 *
 * 调用者：SignalOutput.c -> SetSignalOutputType()
 */
void SetDACWaveTri(void);

/*
 * 切换波形为正弦波（100 点）。
 * 更新内部缓冲区指向正弦波数据。
 *
 * 调用者：SignalOutput.c -> SetSignalOutputType()
 */
void SetDACWaveSine(void);

#endif
