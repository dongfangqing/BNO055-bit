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
#include "stm32f10x.h" 
#include "rtc_driver.h"
#include "usart_driver.h"


/* ���жϱ�־���������ж�ʱ��1����ʱ�䱻ˢ��֮����0 */
__IO uint32_t TimeDisplay = 0;

/*
 * ��������NVIC_Configuration
 * ����  ������RTC���жϵ����ж����ȼ�Ϊ1�������ȼ�Ϊ0
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void RTC_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void RTC_Alarm_EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	
	/* Configure EXTI Line17(RTC Alarm) to generate an interrupt on rising edge */
	EXTI_ClearITPendingBit(EXTI_Line17);
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}



//�·����ݱ�											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�	  
//ƽ����·����ڱ�
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

tm timer;

#if 1
/*
 * ��������RTC_Configuration
 * ����  ������RTC
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void RTC_Configuration(void)
{
	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);

	/* Reset Backup Domain */
//	BKP_DeInit();	//ÿ�ζ���ʼ��BKP�Ļ����Ὣ���ݼĴ�����գ����ᱣ��ֵ�����ԣ���ʼ��һ��֮�󣬰���ע�͵�

	/* Enable LSE */
	RCC_LSEConfig(RCC_LSE_ON);

#ifdef USART1_DEBUG	
	printf("Wait till LSE is ready\r\n");
#endif
	
	/* Wait till LSE is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{
	}
#ifdef USART1_DEBUG	
	printf(" Out Wait till LSE is ready\r\n");
#endif
	
	/* Select LSE as RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);  

	/* Enable RTC Clock */
	RCC_RTCCLKCmd(ENABLE);

	/* Wait for RTC registers synchronization */
	RTC_WaitForSynchro();

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();

	/* Enable the RTC Alarm */  
	RTC_ITConfig(RTC_IT_ALR, ENABLE);

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();

	/* Set RTC prescaler: set RTC period to 1sec */
	RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
}
#else

void RTC_Configuration(void)
{
	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);
	
	/* Reset Backup Domain */
//	BKP_DeInit();
	
	/* Enable LSE */
	RCC_LSEConfig(RCC_LSE_ON);
	
	/* Wait till LSE is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{}
	
	/* Select LSE as RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	/* Enable RTC Clock */
	RCC_RTCCLKCmd(ENABLE);
	
	/* Wait for RTC registers synchronization 
	 * ��ΪRTCʱ���ǵ��ٵģ��ڻ�ʱ���Ǹ��ٵģ�����Ҫͬ��
	 */
	RTC_WaitForSynchro();
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	
	/* Enable the RTC Second */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	
	/* Set RTC prescaler: set RTC period to 1sec */
	RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) = 1HZ */
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
}

#endif

/************************************************
**�������ܣ��ж����������Ƿ�Ϊ����
**����	ֵ��0-----��������	1-----������
*************************************************/
u8 Is_Leap_Year(u16 year)
{			  
			   
	if(year%4 == 0) //�����ܱ�4����
	{ 
		if(year%100 == 0) 
		{ 
			if(year%400 == 0)
				return 1;//�����00��β,��Ҫ�ܱ�400���� 	   
			else 
				return 0;   
		}
		else 
			return 1;   
	}
	else 
		return 0;	
}	


/*********************************************************
**�������ܣ�����RTCʱ��,�������ʱ��ת��Ϊ����
**			��1970��1��1��Ϊ��׼,1970~2099��Ϊ�Ϸ����
**����	ֵ:0,�ɹ�;����:�������.
**********************************************************/
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{		   
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)
		return 1;	   
	
	for(t=1970;t<syear;t++)	//��������ݵ��������
	{
		if(Is_Leap_Year(t))
			seccount+=31622400;//�����������
		else seccount+=31536000;			  //ƽ���������
	}
	
	smon-=1;
	
	for(t=0;t<smon;t++)	   //��ǰ���·ݵ����������
	{
		seccount+=(u32)mon_table[t]*86400;//�·����������
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//����2�·�����һ���������	   
	}
	
	seccount+=(u32)(sday-1)*86400;//��ǰ�����ڵ���������� 
	seccount+=(u32)hour*3600;//Сʱ������
    seccount+=(u32)min*60;	 //����������
	seccount+=sec;//�������Ӽ���ȥ
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP | RCC_APB1Periph_PWR,ENABLE); 
	PWR_BackupAccessCmd(ENABLE);
	printf("SETUP Wait\r\n");
	//����ʱ��
    RTC_WaitForLastTask(); 
	RTC_SetCounter(seccount);
    RTC_WaitForLastTask();
	printf("SETUP succeed\r\n");
	return 0;	    
}


/***************************************************
**�������ܣ����빫�����ڵõ�����(ֻ����1901-2099��)��
**			������������ڼ�
**������������������� 
**����	ֵ�����ں�	
****************************************************/
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{	
	u16 temp2;
	u8 yearH,yearL;
	
	yearH=year/100;	yearL=year%100; 
	// ���Ϊ21����,�������100  
	if (yearH>19)yearL+=100;
	// ����������ֻ��1900��֮���  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7);
}


/*****************************************************
**�������ܣ���ȡ��ǰʱ��
**����	ֵ��0,�ɹ�;����:�������.
******************************************************/
u8 RTC_Get(void)
{
	static u16 daycnt=0;
	u32 timecount=0; 
	u32 temp=0;
	u16 temp1=0;	  
	   
     
    timecount = RTC_GetCounter();			 

	temp=timecount/86400;   //�õ�����(��������Ӧ��)
	if(daycnt!=temp)//����һ����
	{	  
		daycnt=temp;
		temp1=1970;	//��1970�꿪ʼ
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))//������
			{
				if(temp>=366)temp-=366;//�����������
				else {temp1++;break;}  
			}
			else temp-=365;	  //ƽ�� 
			temp1++;  
		}   
		timer.w_year=temp1;//�õ����
		temp1=0;
		while(temp>=28)//������һ����
		{
			if(Is_Leap_Year(timer.w_year)&&temp1==1)//�����ǲ�������/2�·�
			{
				if(temp>=29)temp-=29;//�����������
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//ƽ��
				else break;
			}
			temp1++;  
		}
		timer.w_month=temp1+1;//�õ��·�
		timer.w_date=temp+1;  //�õ����� 
	}
	temp = timecount%86400;     //�õ�������   	   
	timer.hour = temp/3600;     //Сʱ
	timer.min = (temp%3600)/60; //����	
	timer.sec = (temp%3600)%60; //����
	timer.week = RTC_Get_Week(timer.w_year,timer.w_month,timer.w_date);//��ȡ����   
	return 0;
}


/*
 * ��������RTC_CheckAndConfig
 * ����  ����鲢����RTC
 * ����  �����ڶ�ȡRTCʱ��Ľṹ��ָ��
 * ���  ����
 * ����  ���ⲿ����
 */
#if 1
void RTC_CheckAndConfig(tm *tm)
{
	/*������ʱ��鱸�ݼĴ���BKP_DR1��������ݲ���0xA5A5,
	������������ʱ�䲢ѯ���û�����ʱ��*/
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
#ifdef USART1_DEBUG	
		printf("\r\n\r\n RTC not yet configured....");
#endif
		/* RTC Configuration */
		RTC_Configuration();

#ifdef USART1_DEBUG	
		printf("\r\n\r\n RTC configured....");
#endif
		
		RTC_Set(tm->w_year,tm->w_month,tm->w_date,tm->hour,tm->min,tm->sec);

		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}
	else
	{
		/*��������������ʱ��*/
		/*����Ƿ��������*/
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
#ifdef USART1_DEBUG	
			printf("\r\n\r\n Power On Reset occurred....");
#endif
		}
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)	/*����Ƿ�Reset��λ*/
		{
#ifdef USART1_DEBUG	
			printf("\r\n\r\n External Reset occurred....");
#endif
		}

#ifdef USART1_DEBUG	
		printf("\r\n No need to configure RTC....");
#endif	
	
		/*�ȴ��Ĵ���ͬ��*/
		RTC_WaitForSynchro();

		/*����RTC���ж�*/
		RTC_ITConfig(RTC_IT_SEC, ENABLE);

		/*�ȴ��ϴ�RTC�Ĵ���д�������*/
		RTC_WaitForLastTask();
	}
	/* Clear reset flags */
	RCC_ClearFlag();
}

#else

void RTC_CheckAndConfig(tm *tm)
{
   	  /*������ʱ��鱸�ݼĴ���BKP_DR1��������ݲ���0xA5A5,
	  ������������ʱ�䲢ѯ���û�����ʱ��*/
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
		printf("\r\n\r\n RTC not yet configured....");

		/* RTC Configuration */
		RTC_Configuration();
		
		printf("\r\n\r\n RTC configured....");

		/* Adjust time by users typed on the hyperterminal */
//		Time_Adjust(tm);

		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}
	else
	{
	  /*��������������ʱ��*/
		/*����Ƿ��������*/
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
		    printf("\r\n\r\n Power On Reset occurred....");
		}
		/*����Ƿ�Reset��λ*/
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
			printf("\r\n\r\n External Reset occurred....");
		}
	
		printf("\r\n No need to configure RTC....");
		
		/*�ȴ��Ĵ���ͬ��*/
		RTC_WaitForSynchro();
		
		/*����RTC���ж�*/
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		
		/*�ȴ��ϴ�RTC�Ĵ���д�������*/
		RTC_WaitForLastTask();
	}
	   /*������ʱ������꣬������У��ʱ�������PC13*/
	#ifdef RTCClockOutput_Enable
	  /* Enable PWR and BKP clocks */
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	  /* Allow access to BKP Domain */
	  PWR_BackupAccessCmd(ENABLE);
	
	  /* Disable the Tamper Pin */
	  BKP_TamperPinCmd(DISABLE); /* To output RTCCLK/64 on Tamper pin, the tamper
	                                 functionality must be disabled */
	
	  /* Enable RTC Clock Output on Tamper Pin */
	  BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
	#endif

	  /* Clear reset flags */
	  RCC_ClearFlag();

}

#endif
/************************************************************
**��������	:ʶ��ǰ�����硢���绹������
*************************************************************/
u8 Get_AMPM(void)
{
	if(RTC_Get() == 0)
	{
#ifdef USART1_DEBUG	
		printf("Get RTC finish\r\n");
		printf("%d�� %d�� %d�� %d:%d:%d Week: %d \r\n",timer.w_year,timer.w_month,
					timer.w_date,timer.hour,timer.min,timer.sec,timer.week);
#endif
	}

	if((timer.hour >= 5)&&(timer.hour < 12))			//����
		return Froenoon;
	else if((timer.hour >= 12)&&(timer.hour < 17))	//����
		return Noon;
	else if(((timer.hour >= 17)&&((timer.hour <= 23)&&(timer.min <= 59)))||(/*(timer.hour >=0)&&*/(timer.hour < 5)))//����
		return Afternoon;

	return 0;
}


/*************************************************************
**�������ܣ�������һ������ʱ��
**��	����s------�루�룩
**************************************************************/
void Set_RTC_SetAlarm(u32 s)
{
	RTC_WaitForLastTask();
	RTC_SetAlarm(RTC_GetCounter() + s);
    RTC_WaitForLastTask();
}


/*************************************************************
**�������ܣ�ָ��ʱ��������һ������ʱ��
**��	����hour------Сʱ	min-----����
**************************************************************/
void Set_RTC_Alarm(u8 hour,u8 min)
{
	u32 s;
	s8 hour_num;
	s8 min_num;
	
//	if((hour >=0)&&(hour <24)&&(min >= 0)&&(min < 60))
	if((hour <24)&&(min < 60))
	{
		RTC_Get();	//��ȡ��ǰʱ��

		if(hour > timer.hour)
			hour_num = hour - timer.hour -1;
		else
		{
			hour_num =hour + (24 - timer.hour);
			hour_num -= 1; 
		}
		
		min_num = min + (60 - timer.min);

		if(min_num >= 60)
		{
			min_num = min_num - 60;
			hour_num += 1;
		}

		s = (hour_num*60 + min_num)*60;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP | RCC_APB1Periph_PWR,ENABLE); 
		PWR_BackupAccessCmd(ENABLE);
		RTC_WaitForLastTask();
		RTC_SetAlarm(RTC_GetCounter() + s);
		RTC_WaitForLastTask();
	}
}

tm systmtime = 
{
	23,
	15,
	00,
	2016,
	5,
	21,
	6
};

void RTC_init(void)
{
	RTC_NVIC_Config();
	RTC_Alarm_EXTI_Configuration();
//	RTC_Configuration();
	
	RTC_CheckAndConfig(&systmtime);
}



/***************************************************************
**��������:����������,�ж��Ƿ�Ϸ�
u8 const month_tab1[] = {31,28,31,30,31,30,31,31,30,31,30,31};		   ///<������ÿ�µ�����
u8 const month_tab2[] = {31,29,31,30,31,30,31,31,30,31,30,31};		   ///<����ÿ�µ�����

****************************************************************/
uint8_t Judge_CalendarDate(tm *CalendarDate_val)
{
	if(CalendarDate_val->w_year < 1970)
		CalendarDate_val->w_year = 1970;
	if(CalendarDate_val->w_year > 2099)
		CalendarDate_val->w_year = 2099;
	
	switch(CalendarDate_val->w_month)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			if((CalendarDate_val->w_date > 0)&&(CalendarDate_val->w_date <= 31))
				return 0;
			else 
			{
				CalendarDate_val->w_date = 1;
//				return 1;
			}
			break;
		case 2:
			if(Is_Leap_Year(CalendarDate_val->w_year) == 0)	//ƽ�� 
			{
				if((CalendarDate_val->w_date > 0)&&(CalendarDate_val->w_date <= 28))
					return 0;
				else 
				{
					CalendarDate_val->w_date = 1;
					return 1;
				}
			}
			else if(Is_Leap_Year(CalendarDate_val->w_year) == 1)	//����
			{
				if((CalendarDate_val->w_date > 0)&&(CalendarDate_val->w_date <= 29))
					return 0;
				else 
				{
					CalendarDate_val->w_date = 1;
					return 1;
				}
			}
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			if((CalendarDate_val->w_date > 0)&&(CalendarDate_val->w_date <= 30))
				return 0;
			else 
			{
				CalendarDate_val->w_date = 1;
//				return 1;
			}
			break;
	}
	
	return 1;
}

/***************************************************************
**��������:��������ʱ��,�Ƚϴ�С
** ��	��:
**����	ֵ:���timer_1����timer_2,����1;���timer_1С��timer_2,����-1;������,����0
****************************************************************/
int8_t Judge_Timestamp(tm timer_1,tm timer_2)
{
	uint32_t sec_1 = 0;
	uint32_t sec_2 = 0;
	
	sec_1 = timer_1.hour*60*60 + timer_1.min*60 + timer_1.sec;
	sec_2 = timer_2.hour*60*60 + timer_2.hour*60 + timer_2.sec;
	
	if(sec_1 > sec_2)
		return 1;
	else if(sec_1 < sec_2)
		return -1;
	else if(sec_1 == sec_2)
		return 0;
	
	return 0;
}





