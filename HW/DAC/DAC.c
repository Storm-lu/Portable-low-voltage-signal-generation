#include "DAC.h"
#include "Wave.h"
#include "stm32f10x_conf.h"


#define DAC_DHR12R1_ADDR    ((u32)0x40007408)


static StructDACWave s_strDAC1WaveBuf;


static  void ConfigTimer4(u16 arr, u16 psc);
static  void ConfigDAC1(void);
static  void ConfigDMA2Ch3ForDAC1(StructDACWave wave);


static  void ConfigTimer4(u16 arr, u16 psc)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;


  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);


  TIM_DeInit(TIM4);
  TIM_TimeBaseStructure.TIM_Period        = arr;
  TIM_TimeBaseStructure.TIM_Prescaler     = psc;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Update);

  TIM_Cmd(TIM4, ENABLE);
}


static  void ConfigDAC1(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  DAC_InitTypeDef   DAC_InitStructure;


  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);


  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T4_TRGO;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);

  DAC_SetChannel1Data(DAC_Align_12b_R, 0);

  DAC_Cmd(DAC_Channel_1, ENABLE);
}


static  void ConfigDMA2Ch3ForDAC1(StructDACWave wave)
{
  DMA_InitTypeDef   DMA_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);


  DMA_DeInit(DMA2_Channel3);
  DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R1_ADDR;
  DMA_InitStructure.DMA_MemoryBaseAddr     = wave.waveBufAddr;
  DMA_InitStructure.DMA_BufferSize         = wave.waveBufSize;
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
  DMA_Init(DMA2_Channel3, &DMA_InitStructure);

  DMA_Cmd(DMA2_Channel3, ENABLE);
}

void InitDAC(void)
{
  s_strDAC1WaveBuf.waveBufAddr  = (u32)GetTriWave100PointAddr();
  s_strDAC1WaveBuf.waveBufSize  = 100;

  ConfigDAC1();

  ConfigTimer4(71, 99); //100Hz采样率
}


void SetDACWave(StructDACWave wave)
{
  s_strDAC1WaveBuf = wave;
}

void StartDAC(u16 dacValue)
{
  DAC_DMACmd(DAC_Channel_1, DISABLE);
  DMA_Cmd(DMA2_Channel3, DISABLE);

  //dacValue 1.0V -> 1241, 2.0V -> 2482
  DAC_SetChannel1Data(DAC_Align_12b_R, dacValue);
}

void StartDACWave()
{
  ConfigDMA2Ch3ForDAC1(s_strDAC1WaveBuf);
  DAC_DMACmd(DAC_Channel_1, ENABLE);
}

void StopDAC(void)
{
  DAC_DMACmd(DAC_Channel_1, DISABLE);
  DMA_Cmd(DMA2_Channel3, DISABLE);
  
  DAC_SetChannel1Data(DAC_Align_12b_R, 0);
}

void SetDACWaveTri(void){
  s_strDAC1WaveBuf.waveBufAddr  = (u32)GetTriWave100PointAddr();
  s_strDAC1WaveBuf.waveBufSize  = 100;
}

void SetDACWaveSine(void){
  s_strDAC1WaveBuf.waveBufAddr  = (u32)GetSineWave100PointAddr();
  s_strDAC1WaveBuf.waveBufSize  = 100;
}