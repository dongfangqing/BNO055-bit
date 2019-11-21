#ifndef _ADC_DRIVER_H_
#define _ADC_DRIVER_H_

#include "stm32f10x.h" 



void ADC1_Init(void);
uint16_t Get_Adc(uint8_t channel);
float Get_AdcMath(u16 ch);
float Get_Voltage_Value(float value);
float ADC_Byte_Tranform(float flag);



void ADC1_Disable(void);

#endif

