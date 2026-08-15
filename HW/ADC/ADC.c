#include "ADC.h"
#include "stm32f10x_conf.h"

/*
 * ADC 采样值的 DMA 循环缓冲区。
 * DMA1 通道 1 将 ADC 转换结果持续写入此处。
 * 大小：ADC_BUF_SIZE（128）个采样值。
 */
static u16 s_arrADCSamples[ADC_BUF_SIZE];


/* ---- 内部函数声明 ---- */
static void ConfigADC1(void);
static void ConfigDMA1Ch1(void);


/*
 * 配置 ADC1 通道 1（PA1）。
 *
 * 所需设置：
 *   - ADC 时钟：RCC_PCLK2_Div6 -> 72MHz/6 = 12MHz
 *   - PA1：GPIO_Mode_AIN（模拟输入）
 *   - ADC 模式：独立（单通道）
 *   - 扫描模式：禁止（单通道）
 *   - 连续转换：使能（每次转换后自动重启）
 *   - 外部触发：无（软件启动）
 *   - 数据对齐：右对齐（12 位在低位）
 *   - 通道：ADC_Channel_1，转换顺序第 1，采样时间 239.5 周期
 *   - 使能 ADC DMA
 *   - 使能 ADC
 *   - 运行校准（复位 + 等待 + 启动 + 等待）
 *   - 启动软件转换
 *
 * ADC 采样率 = ADC 时钟 / (采样时间 + 转换时间)
 *   = 12MHz / (239.5 + 12.5) = 12MHz / 252 ≈ 47.6kHz
 */
static void ConfigADC1(void)
{
    /* 实现 ADC1 + PA1 配置 */
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);

    ADC_Cmd(ADC1, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1));

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}


/*
 * 配置 DMA1 通道 1，将 ADC 结果传输至存储器。
 *
 * 所需设置：
 *   - 方向：外设 -> 存储器
 *   - 外设地址：&ADC1->DR（ADC 数据寄存器）
 *   - 存储器地址：s_arrADCSamples
 *   - 缓冲区大小：ADC_BUF_SIZE（128）
 *   - 模式：循环（持续采集）
 *   - 数据大小：两者均为半字（16 位）
 *   - 外设地址自增：禁止，存储器地址自增：使能
 *   - 优先级：中等
 */
static void ConfigDMA1Ch1(void)
{
    /* 实现 DMA1 通道 1 配置 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)s_arrADCSamples;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = ADC_BUF_SIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel1, ENABLE);
}


/*
 * 初始化 ADC 系统。
 * 调用 ConfigADC1()，然后调用 ConfigDMA1Ch1()。
 */
void InitADC(void)
{
    /* 初始化 ADC 系统 */
    ConfigADC1();
    ConfigDMA1Ch1();
}


/*
 * 返回指向 ADC 采样缓冲区（128 个采样值，u16）的指针。
 * 缓冲区由 DMA 在循环模式下持续更新。
 *
 * 调用者：SignalMeasure.c -> DisplaySignalMeasure()
 */
u16* GetADCBuf(void)
{
    /* 返回 s_arrADCSamples 指针 */
    return (u16*)s_arrADCSamples;
}


/*
 * 返回最新的 ADC 采样值。
 * 读取 DMA1 通道 1 的当前数据计数器以找到最新写入位置。
 *
 * 调用者：（可选，用于单次采样读取）
 */
u16 GetADCLatest(void)
{
    /* 读取 DMA 计数器，计算最新位置，返回采样值 */
    u16 nextIndex = DMA_GetCurrDataCounter(DMA1_Channel1);
    u16 index = ADC_BUF_SIZE - nextIndex;
    if(index == 0){
        index = ADC_BUF_SIZE - 1;
    }else{
        index--;   
    }
    return s_arrADCSamples[index];
}


/*
 * 计算整个 ADC 缓冲区的统计信息。
 *
 * 参数均为输出指针：
 *   pAvg：所有 128 个采样值的平均值（0~4095）
 *   pMax：找到的最大值（0~4095）
 *   pMin：找到的最小值（0~4095）
 *   pP2P：峰峰值 = 最大值 - 最小值（0~4095）
 *
 * 电压转换：电压_mV = 值 * 3300 / 4095
 *
 * 调用者：SignalMeasure.c、Main.c（Proc1SecTask）
 */
void GetADCStats(u16 *pAvg, u16 *pMax, u16 *pMin, u16 *pP2P)
{
    /* 遍历 s_arrADCSamples，计算平均值/最大值/最小值/峰峰值 */
    u32 sum = 0;
    u16 max = 0;
    u16 min = 0xFFFF;
    u16 p2p = 0;

    for(u16 i = 0; i < ADC_BUF_SIZE; i++){
        u16 val = s_arrADCSamples[i];
        sum += val;
        if(val > max){
        max = val;
        }
        if(val < min){
        min = val;
        }
    }

    p2p = max - min;

    *pAvg = (u16)(sum / ADC_BUF_SIZE);
    *pMax = max;
    *pMin = min;
    *pP2P = p2p;  /* [FIX] 原为 2 * p2p，峰峰值被翻倍 */
}
