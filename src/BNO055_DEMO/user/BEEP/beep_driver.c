/*********************************iBoxV300*********************************                                      
 
	                         \\\|///
                       \\  - -  //
                        (  @ @  )
+---------------------oOOo-(_)-oOOo-------------------------+
|                                                           |
|                             Oooo                          |
+-----------------------oooO--(   )-------------------------+
                       (   )   ) /
                        \ (   (_/
                         \_)           
***************************************************************************/
#include "beep_driver.h"


/*******************************************************
**函数功能：蜂鸣器初始化
********************************************************/
void Beep_InitConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); 
	
	//蜂鸣器控制引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_SetBits(GPIOC, GPIO_Pin_3);
	
}	


void Beep_Contrl(u8 status)
{
	if(status == BEEP_ON)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_3);
	}
	else if(status == BEEP_OFF)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_3);
	}
}

