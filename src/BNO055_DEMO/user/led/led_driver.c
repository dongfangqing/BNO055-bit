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
#include "led_driver.h"  

#if 1
/*********************************************************************
******************************LED灯定义*******************************
**LED_R-----PC6
**LED_G-----PC7
**LED_B-----PC8
**********************************************************************/

/**********************************************************
**函数功能：LED端口初始化
***********************************************************/
void LED_GPIO_Config(void)
{
	//定义一个GPIO_InitTypeDef类型的结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
// 	u16 CCR1_Val = 32;  // PWM信号电平跳变值 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE); 

	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); //Timer1完全重映射  TIM1_CH1->PC6 CH2->PC7 CH1->PC8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	/* ----------------------------------------------------------------------- 
    TIM2 Channe2 duty cycle = (TIM2_CCR2/ TIM2_ARR+1)* 100% = PWM占空比
  ----------------------------------------------------------------------- */

	// Time base configuration 	 
	TIM_TimeBaseStructure.TIM_Period = 255;       //当定时器从0计数到255，即为256次，为一个定时周期
	TIM_TimeBaseStructure.TIM_Prescaler = 1999;	    //设置预分频：2000预分频，即为36KHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0 ;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	// PWM1 Mode configuration: Channe2 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
	TIM_OCInitStructure.TIM_Pulse = 0;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 //使能通道2
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);	 //使能通道2
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 //使能通道2
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3, ENABLE);
	// TIM4 enable counter 
	TIM_Cmd(TIM3, ENABLE);                   //使能定时器2

	TIM3->BDTR|=0x8000;  //使能定时器3输出

	//关闭所有led灯
//	GPIO_SetBits(GPIOC, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8);
}




/**********************************************************
**函数功能：全部点亮LED
***********************************************************/
void LED_All_ON(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8);
}

/**********************************************************
**函数功能：全部熄灭LED
***********************************************************/
void LED_All_OFF(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8);
}


void PWM_Channel1_Out(u8 pwm)
{
	TIM3->CCR1 =  pwm * 10;
}


void PWM_Channel2_Out(u8 pwm)
{
	TIM3->CCR2 =  pwm * 10;
}

void PWM_Channel3_Out(u8 pwm)
{
	TIM3->CCR3 =  pwm * 10;
}

#endif
#if 1

/* LED亮度等级 PWM表 */
uint8_t indexWave[] = {1,1,2,2,3,4,6,8,10,14,19,25,33,44,59,80,
	107,143,191,255,255,191,143,107,80,59,44,33,25,19,14,10,8,6,4,3,2,2,1,1};

 /**
  * @brief  配置TIM3复用输出PWM时用到的I/O
  * @param  无
  * @retval 无
  */
static void TIM3_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIOC clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 

  /* 配置呼吸灯用到的PB0引脚 */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOC, &GPIO_InitStructure);
}


/**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Config_PWM(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* 配置TIM3_IRQ中断为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  配置TIM3输出的PWM信号的模式，如周期、极性
  * @param  无
  * @retval 无
  */

/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> 中断 & TIMxCNT 重新计数
 *                    TIMx_CCR(电平发生变化)
 *
 * 信号周期=(TIMx_ARR +1 ) * 时钟周期
 * 
 */

/*    _______    ______     _____      ____       ___        __         _
 * |_|       |__|      |___|     |____|    |_____|   |______|  |_______| |________|
 */
static void TIM3_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;																				
	
	/* 设置TIM3CLK 时钟为72MHZ */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 					//使能TIM3时钟

  /* 基本定时器配置 */		 
  TIM_TimeBaseStructure.TIM_Period = 255;       							  //当定时器从0计数到255，即为266次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Prescaler = 1999;	    							//设置预分频：
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;			//设置时钟分频系数：不分频(这里用不到)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//向上计数模式
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM模式配置 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    				//配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//使能输出
  TIM_OCInitStructure.TIM_Pulse = 0;										  			//设置初始PWM脉冲宽度为0	
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  	  //当定时器计数值小于CCR1_Val时为低电平

  TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 									//使能通道3
	

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);						//使能预装载	

  TIM_ARRPreloadConfig(TIM3, ENABLE);			 										//使能TIM3重载寄存器ARR

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);                   										//使能定时器3	
	
	TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);										//使能update中断
		
	NVIC_Config_PWM();																					//配置中断优先级		
	
}

/**
  * @brief  TIM3 呼吸灯初始化
  *         配置PWM模式和GPIO
  * @param  无
  * @retval 无
  */
void TIM3_Breathing_Init(void)
{
	TIM3_GPIO_Config();
	TIM3_Mode_Config();	
}
#endif
