/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#if 0
#include "rtc_driver.h"
#include "usart_driver.h"
#include "beep_driver.h"
#endif
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

void RTCAlarm_IRQHandler(void)
{
#if 0
//	printf("RTCAlarm_IRQHandler\r\n");
    RTC_WaitForSynchro();  
	if(SET == RTC_GetFlagStatus(RTC_IT_ALR))
	{
		
		RTC_ClearFlag(RTC_IT_ALR);		////清除中断标志位
		
		if(EXTI_GetITStatus(EXTI_Line17))
		{
			EXTI_ClearITPendingBit(EXTI_Line17);     //清除中断标志位
		}	

		RTC_WaitForLastTask();  
		RTC_ClearITPendingBit(RTC_IT_ALR);  
		RTC_WaitForLastTask(); 
		
		Beep_Contrl(BEEP_ON);
	}
#endif
}

void RTC_IRQHandler(void)
{
	#if 0
//	printf("RTCAlarm_IRQHandler()\r\n");
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
	{
		/* Clear the RTC Second interrupt */
		RTC_ClearITPendingBit(RTC_IT_SEC);

		/* Enable time update */
		TimeDisplay = 1;

		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
	}
	
	if (RTC_GetITStatus(RTC_IT_ALR) != RESET)
	{
		/* Clear the RTC Second interrupt */
		RTC_ClearITPendingBit(RTC_IT_ALR);

		Beep_Contrl(BEEP_ON);

		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
	}
	#endif
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	
}

void EXTI0_IRQHandler(void)
{
	
}

void EXTI2_IRQHandler(void)
{
	
}

void EXTI4_IRQHandler(void)
{

}

//extern IIC_Type_Def IIC_Structure;
void EXTI9_5_IRQHandler(void)
{
}

void EXTI15_10_IRQHandler(void)
{
	
}


uint8_t ch;

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
void USART1_IRQHandler(void)
{
	
}


void USART3_IRQHandler(void)
{
#if 1
#ifdef USART3_ENABLE
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		*(Usart3_RecvStat.Recv_end) = USART_ReceiveData(USART3);
		
		Usart3_RecvStat.Recv_end++;
		Usart3_RecvStat.Recvcount++;
		
		if(USART3_BUFF <= Usart3_RecvStat.Recvcount)
		{
			Usart3_RecvStat.Recvcount = 0;
			Usart3_RecvStat.Recv_end = &Usart3_buff[0];
		}
//	  	printf( "%c", Usart3_buff[Usart2_RecvStat.Recvcount-1]);    //将接受到的数据直接返回打印
//		usart3_putc(Usart3_buff[Usart2_RecvStat.Recvcount-1]);
		
		USART_ClearITPendingBit (USART3, USART_IT_RXNE);
	} 
#endif
#endif
}

void TIM2_IRQHandler(void)
{
	
}


void TIM3_IRQHandler(void)
{	
	
}


void SPI1_IRQHandler(void)
{
	
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
