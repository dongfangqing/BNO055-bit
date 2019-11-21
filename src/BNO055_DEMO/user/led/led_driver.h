#ifndef _LED_H_
#define _LED_H_

#include "stm32f10x.h"

/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON  0
#define OFF 1

/* ���κ꣬��������������һ��ʹ�� */
#define BLUE(a)	if (a)	\
					GPIO_SetBits(GPIOC,GPIO_Pin_6);\
					else		\
					GPIO_ResetBits(GPIOC,GPIO_Pin_6)

#define GREEN(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_7);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_7)
					
#define RED(a)	if (a)	\
					GPIO_SetBits(GPIOC,GPIO_Pin_8);\
					else		\
					GPIO_ResetBits(GPIOC,GPIO_Pin_8)					

	
					

/* ֱ�Ӳ����Ĵ����ķ�������IO */
//#define	digitalHi(p,i)			{p->BSRR=i;}			//����Ϊ�ߵ�ƽ		
//#define digitalLo(p,i)			{p->BRR=i;}				//����͵�ƽ
//#define digitalToggle(p,i)		{p->ODR ^=i;}			//�����ת״̬


///* �������IO�ĺ� */
//#define LED_TOGGLE		digitalToggle(GPIOE,GPIO_Pin_2)
//#define LED_OFF		digitalHi(GPIOE,GPIO_Pin_2)
//#define LED_ON			digitalLo(GPIOE,GPIO_Pin_2)

void LED_GPIO_Config(void);


void LED_All_ON(void);
void LED_All_OFF(void);	


void PWM_Channel1_Out(u8 pwm);
void PWM_Channel2_Out(u8 pwm);
void PWM_Channel3_Out(u8 pwm);

void TIM3_Breathing_Init(void);


#endif
