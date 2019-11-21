#ifndef _BEEP_DRIVER_H_
#define _BEEP_DRIVER_H_


#include "stm32f10x.h" 

#define BEEP_ON			1
#define BEEP_OFF		0


void Beep_InitConfig(void);
void Beep_Contrl(u8 status);
void PWM_Channel2_Out(u8 pwm);


void BEEP_PowerMode_Port_GPIO_ReInit(void);


#endif


