#include "DAC.h"
#include "Wave.h"
#include "stm32f10x_conf.h"


/*
 * DAC1 12 位右对齐数据保持寄存器地址。
 * DMA 将波形采样数据写入此寄存器。
 */
#define DAC_DHR12R1_ADDR    ((u32)0x40007408)


/*
 * 当前波形缓冲区描述符。
 * 由 SetDACWaveTri/SetDACWaveSine 更新，由 StartDACWave 读取。
 */
static StructDACWave s_strDAC1WaveBuf;


/* ---- 内部函数声明 ---- */
static void ConfigTimer4(u16 arr, u16 psc);
static void ConfigDAC1(void);
static void ConfigDMA2Ch3ForDAC1(StructDACWave wave);


/*
 * 配置 TIM4 作为 DAC 触发源。
 *
 * 时钟：APB1 = 72MHz
 * 计数频率 = 72MHz / (PSC+1)
 * 触发频率 = 计数频率 / (ARR+1) = DAC 采样率
 *
 * 所需设置：
 *   - TIM_TRGOSource_Update：在每次更新事件时触发 DAC
 *   - 向上计数模式
 *
 * 示例：arr=99, psc=71 -> 72MHz/72/100 = 10kHz 采样率
 *       搭配 100 点波形 -> 100Hz 输出频率
 */
static void ConfigTimer4(u16 arr, u16 psc) //Auto-reload Register & Prescaler
{
    /* 实现 TIM4 配置 */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    TIM_DeInit(TIM4);
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Update);

    TIM_Cmd(TIM4, ENABLE);
}


/*
 * 配置 DAC 通道 1 输出（PA4）。
 *
 * 所需设置：
 *   - PA4：GPIO_Mode_AIN（模拟输入）
 *   - DAC 触发：DAC_Trigger_T4_TRGO（由 TIM4 触发）
 *   - DAC 输出缓冲：使能
 *   - 初始数据：0
 *   - 使能 DAC 通道 1
 */
static void ConfigDAC1(void)
{
    /* 实现 DAC1 + PA4 GPIO 配置 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);


    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    DAC_InitTypeDef DAC_InitStructure;
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_T4_TRGO;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);

    DAC_SetChannel1Data(DAC_Align_12b_R, 0);

    DAC_Cmd(DAC_Channel_1, ENABLE);
}


/*
 * 配置 DMA2 通道 3，将波形数据传送至 DAC1。
 *
 * 所需设置：
 *   - 方向：存储器 -> 外设
 *   - 外设地址：DAC_DHR12R1_ADDR (0x40007408)
 *   - 存储器地址：wave.waveBufAddr
 *   - 缓冲区大小：wave.waveBufSize
 *   - 模式：循环（无限循环输出波形）
 *   - 数据大小：外设和存储器均为半字（16 位）
 *   - 外设地址自增：禁止，存储器地址自增：使能
 *   - 优先级：高
 */
static void ConfigDMA2Ch3ForDAC1(StructDACWave wave)
{
    /* 实现 DMA2 通道 3 配置 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
    DMA_DeInit(DMA2_Channel3);

    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R1_ADDR;
    DMA_InitStructure.DMA_MemoryBaseAddr = wave.waveBufAddr;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = wave.waveBufSize;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    DMA_Init(DMA2_Channel3, &DMA_InitStructure);
    DMA_Cmd(DMA2_Channel3, ENABLE);
}

/*
 * 初始化 DAC 系统。
 *
 * 步骤：
 *   1. 设置默认波形为三角波（来自 Wave 模块的 100 点）
 *   2. 调用 ConfigDAC1() 配置 PA4 和 DAC 通道 1
 *   3. 调用 ConfigTimer4(99, 71) 设置 10kHz 采样率（= 100Hz 波形）
 *
 * 注意：此处不要使能 DAC DMA。DMA 稍后由 StartDACWave() 启动。
 */
void InitDAC(void)
{
    /* 初始化 DAC 系统 */
    SetDACWaveTri();
    ConfigDAC1();
    ConfigTimer4(99, 71);
}


/*
 * 更新内部波形缓冲区描述符。
 * 由 SetDACWaveTri/SetDACWaveSine 调用，用于切换波形数据。
 */
void SetDACWave(StructDACWave wave)
{
    /* 更新 s_strDAC1WaveBuf */
    s_strDAC1WaveBuf = wave;
}


/*
 * 在 PA4 上输出直流电压。
 *
 * 步骤：
 *   1. 禁用 DAC DMA 和 DMA2 通道 3（停止任何波形输出）
 *   2. 将 dacValue 直接写入 DAC 数据寄存器
 *
 * dacValue：12 位值（0~4095）
 *   0V -> 0, 1.0V -> 1241, 2.0V -> 2482, 3.3V -> 4095
 */
void StartDAC(u16 dacValue)
{
    /* 实现直流电压输出 */
    DMA_Cmd(DMA2_Channel3, DISABLE);

    DAC_SetChannel1Data(DAC_Align_12b_R, dacValue);
}


/*
 * 在 PA4 上启动 DMA 驱动的波形输出。
 *
 * 步骤：
 *   1. 使用当前波形缓冲区（s_strDAC1WaveBuf）配置 DMA2 通道 3
 *   2. 使能 DAC DMA 请求（DAC_DMACmd）
 *
 * 波形将通过 DMA 循环模式持续循环输出。
 * TIM4 以 InitDAC 中设置的采样率触发 DAC 转换。
 */
void StartDACWave(void)
{
    /* 启动 DMA 驱动的波形输出 */
    ConfigDMA2Ch3ForDAC1(s_strDAC1WaveBuf);
    DAC_DMACmd(DAC_Channel_1, ENABLE);
}


/*
 * 停止所有 DAC 输出。
 *
 * 步骤：
 *   1. 禁用 DAC DMA 请求
 *   2. 禁用 DMA2 通道 3
 *   3. 将 DAC 输出设为 0
 */
void StopDAC(void)
{
    /* 停止 DAC 输出 */
    DAC_DMACmd(DAC_Channel_1, DISABLE);
    DMA_Cmd(DMA2_Channel3, DISABLE);
  
    DAC_SetChannel1Data(DAC_Align_12b_R, 0);
}


/*
 * 切换波形为三角波。
 * 从 Wave 模块获取三角波数据地址并更新缓冲区描述符。
 */
void SetDACWaveTri(void)
{
    /* 设置波形为三角波（100 点） */
    s_strDAC1WaveBuf.waveBufAddr  = (u32)GetTriWave100PointAddr();
    s_strDAC1WaveBuf.waveBufSize  = 100;
}


/*
 * 切换波形为正弦波。
 * 从 Wave 模块获取正弦波数据地址并更新缓冲区描述符。
 */
void SetDACWaveSine(void)
{
    /* 设置波形为正弦波（100 点） */
    s_strDAC1WaveBuf.waveBufAddr  = (u32)GetSineWave100PointAddr();
    s_strDAC1WaveBuf.waveBufSize  = 100;
}
