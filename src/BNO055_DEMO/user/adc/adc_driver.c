#include "adc_driver.h"

/******************************************************************
**ADC�˿�:
**�ɼ��˿�:ADC1  ͨ��10-PC0
**�ض˿�:PB14-----����ʱPB14����͵�ƽ
*******************************************************************/

#define ADC1_DR_Address    ((u32)0x40012400+0x4c) //(uint32_t)&ADC1->DR

__IO uint16_t ADC_ConvertedValue[50][2];//�������ADCת�������Ҳ��DMA��Ŀ���ַ
__IO uint16_t After_filter[2];    //���������ƽ��ֵ֮��Ľ��


/*******************************************
**�������ܣ�����ADC1ͨ��10-PC0,11-PC1
********************************************/
static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable DMA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);

	
	/* Configure PC.0  as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/**
  * @brief  ����ADC1�Ĺ���ģʽΪMDAģʽ
  * @param  ��
  * @retval ��
  */
static void ADC1_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	// DMA channel1 configuration 
	DMA_DeInit(DMA1_Channel1);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;	 //ADC��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;	//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 50*2;	//ÿͨ���ɼ�50��,2��ͨ��
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//�����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  				//�ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//���ݿ��Ϊ����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	//���ݿ��Ϊ16λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;						//ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	// Enable DMA channel0 
	DMA_Cmd(DMA1_Channel1, ENABLE);

	// ADC1 configuration 	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;			//����ADCģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 	//ɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//��������ת��ģʽ������ͣ�ؽ���ADCת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//��ʹ���ⲿ����ת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//�ɼ������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 2;	 			//Ҫת����ͨ����Ŀ2
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/*����ADCʱ�ӣ�ΪPCLK2��8��Ƶ����9MHz*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	/*����ADC1��ͨ��10,11Ϊ239.	5���������ڣ�����Ϊ1 */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_239Cycles5);
	
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	
	/*��λУ׼�Ĵ��� */   
	ADC_ResetCalibration(ADC1);
	/*�ȴ�У׼�Ĵ�����λ��� */
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	/* ADCУ׼ */
	ADC_StartCalibration(ADC1);
	/* �ȴ�У׼���*/
	while(ADC_GetCalibrationStatus(ADC1));
	
	/* ����û�в����ⲿ����������ʹ���������ADCת�� */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/*****************************************************
**�������ܣ�ADC1��ʼ��
******************************************************/
void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
}



//���ADCֵ
//ch:ͨ��ֵ 10	����ֻ�õ�һ��ͨ��
//uint16_t Get_Adc(uint8_t ch)
//{
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	        //�������ADCת��
//	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC )); //�ȴ�ת������
//	return ADC_GetConversionValue(ADC1);
//}

//��ƽ��ֵ����
void filter(u8 num)
{
	int  sum = 0;
	u8  count;    

	for(count=0;count<50;count++)
	{
		sum += ADC_ConvertedValue[count][num];
	}
	After_filter[num]=sum/50;
}

/*
**�������ܣ���ȡXͨ��ADֵ��0~1023��
**����    ��channel---ģ���ͨ����
**����ȡֵ��	10~11
*/
uint16_t Get_Adc(uint8_t channel)
{
	uint8_t ch = 0;
	
	if(channel == 10)
		ch = 0;
	else if(channel == 11)
		ch = 1;
	else return 0;
	
 	filter(ch);
	return 	After_filter[ch];	  	
}



/***************************************
**�������ܣ���ȡ�ɼ���ѹֵ
**��	ʽ��V(sample) = V(REF) * Value/(0x0FFF + 1)
****************************************/
float Get_AdcMath(u16 ch)
{
	u16 temp = ch;
	float Value;

//	Value = temp * 3.3;
//	Value = Value / (0x0FFF + 1);


	Value = (float)temp * (3.3 / 4096);

	return Value;
}

/***********************************
**�������ܣ���ȡiBox�����ѹֵ
************************************/
float Get_Voltage_Value(float value)
{
	float ret = 0.00;
	
	ret = value * 1.5;
	
	return ret;
}

/*
**�������ܣ�������ת������
*/
float ADC_Byte_Tranform(float flag)
{
	float num = 0.00;

	num = flag * 100;

	return num;
}

