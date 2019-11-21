#include "key_driver.h"
#include "SysTick.h"


void Key_GPIOConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);


	/* EXTI line gpio config(PC13 PC2) */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}



/***********************************************
**函数功能	:读取按键引脚状态值
**返回	值	:返回按键值
************************************************/
uint8_t Read_Key_Port(void)
{	
	if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
	{
		return KEY_1;
	}
	else if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1))
	{
		return KEY_2;
	}
	else if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2))
	{
		return KEY_3;
	}
	else if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))
	{
		return KEY_4;
	}

	return 0;
}


uint8_t key_scan(void)
{
	switch(Read_Key_Port())
	{
		case KEY_1:
			delay_ms(30);
			if(Read_Key_Port() == KEY_1)
			{
				while(!(Read_Key_Port() == 0));
				return KEY_1;
			}
			break;
		case KEY_2:
			delay_ms(30);
			if(Read_Key_Port() == KEY_2)
			{
				while(!(Read_Key_Port() == 0));
				return KEY_2;
			}
			break;
		case KEY_3:
			delay_ms(30);
			if(Read_Key_Port() == KEY_3)
			{
				while(!(Read_Key_Port() == 0));
				return KEY_3;
			}
			break;
		case KEY_4:
			delay_ms(30);
			if(Read_Key_Port() == KEY_4)
			{
				while(!(Read_Key_Port() == 0));
				return KEY_4;
			}
			break;
		default:
			break;
	}
	
	return NO_KEY;
}




