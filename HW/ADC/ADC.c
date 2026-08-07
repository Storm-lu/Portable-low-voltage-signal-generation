#include "ADC.h"
#include "stm32f10x_conf.h"
#include "U16Queue.h"

static u16 s_arrADCSamples[ADC_BUF_SIZE];

static u16 s_arrADC1Data;
static StructU16CirQue  s_structADCCirQue;
static u16              s_arrADCBuf[ADC1_BUF_SIZE];


static void ConfigADC1(void);
static void ConfigDMA1Ch1(void);

static void ConfigADC1(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  ADC_InitTypeDef   ADC_InitStructure;


  RCC_ADCCLKConfig(RCC_PCLK2_Div6);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1  , ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);


  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


  ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode       = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel       = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);

  ADC_DMACmd(ADC1, ENABLE);
  ADC_Cmd(ADC1, ENABLE);

  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));

  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}


static void ConfigDMA1Ch1(void)
{
  DMA_InitTypeDef DMA_InitStructure;


  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);


  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);
  DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)s_arrADCSamples;
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize         = ADC_BUF_SIZE;
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;
  DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);

  DMA_Cmd(DMA1_Channel1, ENABLE);
}

void InitADC(void)
{
  ConfigADC1();
  ConfigDMA1Ch1();
}


u8 WriteADCBuf(u16 d)
{
  u8 ok = 0;

  ok = EnU16Queue(&s_structADCCirQue, &d, 1);

  return ok;
}


u8 ReadADCBuf(u16* p)
{
  u8 ok = 0;

  ok = DeU16Queue(&s_structADCCirQue, p, 1);

  return ok;
}

u16* GetADCBuf(void)
{
  return s_arrADCSamples;
}

u16 GetADCLatest(void){
  u16 nextIdx = DMA_GetCurrDataCounter(DMA1_Channel1);
  u16 idx = ADC_BUF_SIZE - nextIdx;
  if(idx == 0){
    idx = ADC_BUF_SIZE - 1;
  }else{
    idx--;
  }
  return s_arrADCSamples[idx];
}

void GetADCStats(u16 *pAvg, u16 *pMax, u16 *pMin, u16 *pP2P){
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
  *pP2P = p2p;
}