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
    /* 待实现：实现 ADC1 + PA1 配置 */
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
    /* 待实现：实现 DMA1 通道 1 配置 */
}


/*
 * 初始化 ADC 系统。
 * 调用 ConfigADC1()，然后调用 ConfigDMA1Ch1()。
 */
void InitADC(void)
{
    /* 待实现：初始化 ADC 系统 */
}


/*
 * 返回指向 ADC 采样缓冲区（128 个采样值，u16）的指针。
 * 缓冲区由 DMA 在循环模式下持续更新。
 *
 * 调用者：SignalMeasure.c -> DisplaySignalMeasure()
 */
u16* GetADCBuf(void)
{
    /* 待实现：返回 s_arrADCSamples 指针 */
    return (u16*)0;
}


/*
 * 返回最新的 ADC 采样值。
 * 读取 DMA1 通道 1 的当前数据计数器以找到最新写入位置。
 *
 * 调用者：（可选，用于单次采样读取）
 */
u16 GetADCLatest(void)
{
    /* 待实现：读取 DMA 计数器，计算最新位置，返回采样值 */
    return 0;
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
    /* 待实现：遍历 s_arrADCSamples，计算平均值/最大值/最小值/峰峰值 */
}
