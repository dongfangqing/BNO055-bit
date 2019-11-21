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
#include "HF_LPT200.h"
#include "usart_driver.h"
#include "ST_string.h"
#include "SysTick.h"

#include <stdlib.h>
#include "STM32_ChipID.h"
#include "beep_driver.h"

#if 1

/*********************************************
**函数功能：向HF_LPT200发送AT指令
**********************************************/
void WIFI_HF_LPT200_Send_data(u8* str)
{
#ifdef USART2_ENABLE
	if(st_strlen((char*)str) == Usart2_Sendstr(str))
	{
#ifdef USART1_DEBUG	
	printf("AT Send : %s\r\n",str);
#endif
		delay_ms(5);
	}
	else
	{
#ifdef USART1_DEBUG	
		printf("AT Send fild\r\n");
#endif
	}
#endif

#ifdef USART3_ENABLE
	if(st_strlen((char*)str) == Usart3_Sendstr(str))
	{
#ifdef USART1_DEBUG	
	printf("AT Send : %s\r\n",str);
#endif
		delay_ms(5);
	}
	else
	{
#ifdef USART1_DEBUG	
		printf("AT Send fild\r\n");
#endif
	}
#endif
}

/**************************************
**函数功能：检测是否从HF_LPT200读取到数据
**************************************/
u8 WIFI_HF_LPT200_Read_data(void)
{
	u8 i = 0;
#ifdef USART2_ENABLE	
	while(!Usart2_RecvStat.Recvcount)
	{
		i++;
		
		delay_ms(2);
		
		if(i>10)
		{
#ifdef USART1_DEBUG	
			printf("Check Recv Data: timeout\r\n");
#endif
			return 1;
		}
	}
#endif

#ifdef USART3_ENABLE	
	while(!Usart3_RecvStat.Recvcount)
	{
		i++;
		
		delay_ms(2);
		
		if(i>10)
		{
#ifdef USART1_DEBUG	
			printf("Check Recv Data: timeout\r\n");
#endif
			return 1;
		}
	}
#endif
	
	return 0;
}

/**************************************************
**函数功能：检测读取是否读取到WIFI_HF_LPT200的数据
***************************************************/
u8 WIFI_HF_LPT200_Check_data(void)
{
	u8 i = 0;

#ifdef USART2_ENABLE		
	while(!Usart2_RecvStat.Recvcount)
	{
		i++;
		
		delay_ms(100);
		
		if(i>10)
		{
#ifdef USART1_DEBUG	
			printf("Check Recv Data: timeout\r\n");
#endif
			return 1;
		}
	}
	
#ifdef USART1_DEBUG	
	printf("AT return: %s\r\n",Usart2_buff);
#endif
#endif

#ifdef USART3_ENABLE		
	while(!Usart3_RecvStat.Recvcount)
	{
		i++;
		
		delay_ms(100);
		
		if(i>10)
		{
#ifdef USART1_DEBUG	
			printf("Check Recv Data: timeout\r\n");
#endif
			return 1;
		}
	}
	
#ifdef USART1_DEBUG	
	printf("AT return: %s\r\n",Usart3_buff);
#endif
#endif

	return 0;
}

/*******************************************************
**函数功能：清空数组缓冲
********************************************************/
void WIFI_HF_LPT200_Clear_RecvData(void)
{
#ifdef USART2_ENABLE	
	Clear_USART_Buff(USART2_BUF);
#endif

#ifdef USART3_ENABLE	
	Clear_USART_Buff(USART3_BUF);
#endif
}


/*******************************************************
**函数功能：发送"+++"进入AT命令模式
*******************************************************/
u8 WIFI_HF_LPT200_trying_In_Command_Mode(void)
{
	s8 num = 0;
#ifdef USART2_ENABLE		
	while(1)
	{
		WIFI_HF_LPT200_Send_data((u8*)"+++");
		num = WIFI_HF_LPT200_Check_data();
		if(num)
		{
#ifdef USART1_DEBUG
			printf("NO OK!!!\r\n");
#endif
			
			return 0;
		}
		
		if('a' == Usart2_buff[0])
		{
			WIFI_HF_LPT200_Clear_RecvData();
			WIFI_HF_LPT200_Send_data((u8*)"a");
			delay_ms(100);
			
			num = WIFI_HF_LPT200_Check_data();
			
			if(num)
				return 0;
			
			if(0 == st_strncmp((s8*)"+ok",Usart2_buff,3))
			{
#ifdef USART1_DEBUG
				printf("TO AT Mode OK!!!\r\n");
#endif
				
				return 1;
			}
			else
				return 0;
		}
		else
			return 0;
	}
#endif


#ifdef USART3_ENABLE		
	while(1)
	{
		WIFI_HF_LPT200_Send_data((u8*)"+++");
		num = WIFI_HF_LPT200_Check_data();
		if(num)
		{
#ifdef USART1_DEBUG
			printf("NO OK!!!\r\n");
#endif
			
			return 0;
		}
		
		if('a' == Usart3_buff[0])
		{
			WIFI_HF_LPT200_Clear_RecvData();
			WIFI_HF_LPT200_Send_data((u8*)"a");
			delay_ms(100);
			
			num = WIFI_HF_LPT200_Check_data();
			
			if(num)
				return 0;
			
			if(0 == st_strncmp((s8*)"+ok",Usart3_buff,3))
			{
#ifdef USART1_DEBUG
				printf("TO AT Mode OK!!!\r\n");
#endif
				
				return 1;
			}
			else
				return 0;
		}
		else
			return 0;
	}
#endif
}

/******************************************************
**函数功能：确认进入AT模式
*******************************************************/
u8 WIFI_HF_LPT200_Enter_AT_Mode(void)
{
	u8 ret;

#ifdef USART2_ENABLE			
	WIFI_HF_LPT200_Send_data((u8*)"AT+\r\n");
	
	ret = WIFI_HF_LPT200_Check_data();
	
	if((0 == st_strncmp((s8*)"+ok",Usart2_buff,3))||
		(0 == st_strncmp((s8*)"AT+",Usart2_buff,3)))
	{
#ifdef USART1_DEBUG
		printf("ENTER AT MODE OK!!!\r\n");
#endif
		return 1;
	}
#endif

#ifdef USART3_ENABLE			
	WIFI_HF_LPT200_Send_data((u8*)"AT+\r\n");
	
	ret = WIFI_HF_LPT200_Check_data();
	
	if((0 == st_strncmp((s8*)"+ok",Usart3_buff,3))||
		(0 == st_strncmp((s8*)"AT+",Usart3_buff,3)))
	{
#ifdef USART1_DEBUG
		printf("ENTER AT MODE OK!!!\r\n");
#endif
		return 1;
	}
#endif
	
	//如果没有进入命令模式，则发送"+++"进入命令模式
	WIFI_HF_LPT200_Clear_RecvData();
	
	ret = WIFI_HF_LPT200_trying_In_Command_Mode();
	
	WIFI_HF_LPT200_Clear_RecvData();
	if(ret)
	{
#ifdef USART1_DEBUG
		printf("ENTER AT MODE2 OK!!!\r\n");
#endif
		return 1;		
	}
	else
	{
#ifdef USART1_DEBUG
		printf("IN AT MODE Fild!!!\r\n");
#endif
		return 0;
	}
}

/***********************************************************
**函数功能：进入AT模式
************************************************************/
u8 WIFI_HF_LPT200_To_ATMode(void)
{
	u8 ret;
	
	WIFI_HF_LPT200_Clear_RecvData();
	
	//发"+++"进入AT命令模式
	ret = WIFI_HF_LPT200_trying_In_Command_Mode();
	
	WIFI_HF_LPT200_Clear_RecvData();
	
	if(ret)
	{
#ifdef USART1_DEBUG
		printf("Enter AT Mode OK!!!\r\n");
#endif
		WIFI_HF_LPT200_Send_data((u8*)"AT+E=off\r\n");//关闭回显
		
		WIFI_HF_LPT200_Check_data();
		WIFI_HF_LPT200_Clear_RecvData();
		
		return 1;
	}
	
	return 0;
}

/************************************************
**函数功能：退出AT模式
*************************************************/
u8 WIFI_HF_LPT200_Leave_ATMode(void)
{
	WIFI_HF_LPT200_Clear_RecvData();
	
	WIFI_HF_LPT200_Send_data((u8*)"AT+ENTM\r\n");
	
	if(WIFI_HF_LPT200_Check_data())
	{
#ifdef USART1_DEBUG
		printf("Leave AT Mode Fild!!!\r\n");
#endif
		
		return 0;
	}
	
#ifdef USART1_DEBUG
		printf("Leave AT Mode OK!!!\r\n");
#endif
	
	return 1;
}

/*************************************************************
**函数功能：检测网络连接状态
**返回	值：1------连接失败
**			0------连接成功
**************************************************************/
u8 WIFI_HF_LPT200_Check_NetWork_Status(void)
{
	u8 i = 20;
	
	WIFI_HF_LPT200_Clear_RecvData();
	//关闭回显
	WIFI_HF_LPT200_Send_data((u8*)"AT+E=off\r\n");
	delay_ms(100);
	WIFI_HF_LPT200_Check_data();
	WIFI_HF_LPT200_Clear_RecvData();
	
	//查询STA的无线Link状态
	WIFI_HF_LPT200_Send_data((u8*)"AT+WSLK\r\n");
	WIFI_HF_LPT200_Check_data();
	WIFI_HF_LPT200_Clear_RecvData();
	
	WIFI_HF_LPT200_Send_data((u8*)"AT+WSLK\r\n");
	while(WIFI_HF_LPT200_Check_data() && i--);

#ifdef USART2_ENABLE		
	if(0 == st_strncmp((s8*)"+ok=Disconnected",Usart2_buff,st_strlen("+ok=Disconnected")))
	{
#ifdef USART1_DEBUG
		printf("WIFI NO Link!!!\r\n");
#endif
		return 1;	//连接失败
	}
#endif

#ifdef USART3_ENABLE		
	if(0 == st_strncmp((s8*)"+ok=Disconnected",Usart3_buff,st_strlen("+ok=Disconnected")))
	{
#ifdef USART1_DEBUG
		printf("WIFI NO Link!!!\r\n");
#endif
		return 1;	//连接失败
	}
#endif
	
	return 0;
}




/*************************************************************
**函数功能：配置WIFI_HF_LPT200为TCP客户端模式
**参	数：IP_Addr------指针指向服务器端口号和IP地址
**			格式：04002,42.62.41.209
**************************************************************/
void WIFI_HF_LPT200_Set_TCPClient_Mode(u8* IP_Addr)
{
	u8 buff[64] = {0};
	u8 loop = 1;
	u8 ret = 0;
	u8* ip_p = IP_Addr;

	if(IP_Addr != (void*)0)
	{
		sprintf((char*)buff,"AT+NETP=TCP,CLIENT,%s\r\n",IP_Addr);
		
		while(loop)
		{
			WIFI_HF_LPT200_Send_data(buff);
			
			loop = WIFI_HF_LPT200_Check_data();
			
			if(loop)
			{
				ret = WIFI_HF_LPT200_To_ATMode();
#ifdef USART1_DEBUG
				if(ret)
				{
					printf("WIFI_HF_LPT200_Set_TCPClient_Mode In AT Mode OK!\r\n");
				}
				else
				{
					printf("WIFI_HF_LPT200_Set_TCPClient_Mode In AT Mode Fild!\r\n");
				}
#endif
			}
		}
		
		WIFI_HF_LPT200_Clear_RecvData();
		
		delay_ms(200);
		//检测网络链接状态
		WIFI_HF_LPT200_Check_NetWork_Status();
		WIFI_HF_LPT200_Check_data();
		
		WIFI_HF_LPT200_Clear_RecvData();
		loop = 1;
		while(*ip_p++ != ',');	//获取服务器IP地址
#ifdef USART1_DEBUG
		printf("Server IP : %s\r\n",ip_p);
#endif
		st_memset((s8*)buff,0,64);
		sprintf((char*)buff,"AT+PING=%s\r\n",ip_p);
		while(loop)
		{
			WIFI_HF_LPT200_Send_data(buff);
			delay_ms(50);
			loop = WIFI_HF_LPT200_Check_data();
		}
		
		WIFI_HF_LPT200_Leave_ATMode();
		WIFI_HF_LPT200_Clear_RecvData();
		
#ifdef USART1_DEBUG
		printf("WIFI a TCP Client Mode!!!\r\n");
#endif
	}
}


/*************************************************************
**函数功能：配置WIFI_HF_LPT200为TCP服务器端模式
**************************************************************/
void WIFI_HF_LPT200_Set_TCPServer_Mode(void)
{
	WIFI_HF_LPT200_Clear_RecvData();
	
	WIFI_HF_LPT200_Send_data((u8*)"AT+NETP=TCP,SERVER,48898,10.10.100.254\r\n");
	WIFI_HF_LPT200_Check_data();
	WIFI_HF_LPT200_Clear_RecvData();
	
	WIFI_HF_LPT200_Leave_ATMode();
	
	WIFI_HF_LPT200_Clear_RecvData();
#ifdef USART1_DEBUG
	printf("WIFI a TCP Server Mode!!!\r\n");
#endif
}

/***********************************************
**函数功能：设置WIFI ID
************************************************/
void WIFI_HF_LPT200_Set_ID(u8* id)
{
	u8 buff[20] = {0};

#ifdef USART2_ENABLE	
	if(id != (void*)0)
	{
		sprintf((char*)buff,"AT+WRMID=%s\r\n",id);
		WIFI_HF_LPT200_Clear_RecvData();
		
		WIFI_HF_LPT200_Send_data(buff);
		
		if(!WIFI_HF_LPT200_Read_data())
		{
			if(0 == st_strncmp((s8*)"+ok",Usart2_buff,3))
			{
#ifdef USART1_DEBUG
				printf("Set WIFI ID : %s\r\n",id);
#endif
			}
		}
	
		WIFI_HF_LPT200_Clear_RecvData();
	}
#endif

#ifdef USART3_ENABLE	
	if(id != (void*)0)
	{
		sprintf((char*)buff,"AT+WRMID=%s\r\n",id);
		WIFI_HF_LPT200_Clear_RecvData();
		
		WIFI_HF_LPT200_Send_data(buff);
		
		if(!WIFI_HF_LPT200_Read_data())
		{
			if(0 == st_strncmp((s8*)"+ok",Usart3_buff,3))
			{
#ifdef USART1_DEBUG
				printf("Set WIFI ID : %s\r\n",id);
#endif
			}
		}
	
		WIFI_HF_LPT200_Clear_RecvData();
	}
#endif

}

/***********************************************
**函数功能：获取WIFI ID
**参	数：id-----存放获取到的ID，长度不超过20Byte
************************************************/
void WIFI_HF_LPT200_Get_ID(u8* id)
{
	u8 i = 0;

#ifdef USART2_ENABLE		
	WIFI_HF_LPT200_Clear_RecvData();
	
	WIFI_HF_LPT200_Send_data((u8*)"AT+MID\r\n");
	
	if(!WIFI_HF_LPT200_Read_data())
	{
		delay_ms(200);
		
		if(0 == st_strncmp((s8*)"+ok=",Usart2_buff,4))
		{
			while(Usart2_buff[i+4] != 0)
			{
				id[i] = Usart2_buff[i+4];
				i++;
			}
			
			id[i] = 0;	//字符串'\0'结束符
			
#ifdef USART1_DEBUG
			printf("WIFI ID : %s\r\n",id);
#endif
		}
	}
	
	WIFI_HF_LPT200_Clear_RecvData();
#endif

#ifdef USART3_ENABLE		
	WIFI_HF_LPT200_Clear_RecvData();
	
	WIFI_HF_LPT200_Send_data((u8*)"AT+MID\r\n");
	
	if(!WIFI_HF_LPT200_Read_data())
	{
		delay_ms(200);
		
		if(0 == st_strncmp((s8*)"+ok=",Usart3_buff,4))
		{
			while(Usart3_buff[i+4] != 0)
			{
				id[i] = Usart3_buff[i+4];
				i++;
			}
			
			id[i] = 0;	//字符串'\0'结束符
			
#ifdef USART1_DEBUG
			printf("WIFI ID : %s\r\n",id);
#endif
		}
	}
	
	WIFI_HF_LPT200_Clear_RecvData();
#endif
}



void Set_HF_LPT200_Work_mode(WIFIWork_Mode mode)
{
#ifdef USART2_ENABLE
	if(WIFI_MODE_AP == mode)
	{
//		Usart2_Sendstr((u8*)"AT+WAP=11BGN,iBox,AUTO\r\n");
		Usart2_Sendstr((u8*)"AT+WMODE=AP\r\n");
	}
	else if(WIFI_MODE_STA == mode)
	{
		Usart2_Sendstr((u8*)"AT+WMODE=STA\r\n");
	}
	else if(WIFI_MODE_APSTA == mode)
	{
		Usart2_Sendstr((u8*)"AT+WMODE=APSTA\r\n");
	}
#endif
	
#ifdef USART3_ENABLE
	if(WIFI_MODE_AP == mode)
	{
//		Usart3_Sendstr((u8*)"AT+WAP=11BGN,iBox,AUTO\r\n");
		Usart3_Sendstr((u8*)"AT+WMODE=AP\r\n");
	}
	else if(WIFI_MODE_STA == mode)
	{
		Usart3_Sendstr((u8*)"AT+WMODE=STA\r\n");
	}
	else if(WIFI_MODE_APSTA == mode)
	{
		Usart3_Sendstr((u8*)"AT+WMODE=APSTA\r\n");
	}
#endif
}

/*******************************************************************
**函数功能：设置用户名和密码
********************************************************************/
u8 WIFI_HF_LPT200_Set_Args(u8* ssid,u8* passwd)
{
	u8 buff[64] = {0};
	u8 i = 0;

	if((ssid != (void*)0)&&(passwd != (void*)0))
	{
		if(WIFI_HF_LPT200_Enter_AT_Mode())
		{
#ifdef USART1_DEBUG
			printf("WifiSmartlink:Enter At Mode Sucessfully\r\n");
#endif
		}
		else
		{
#ifdef USART1_DEBUG
			printf("WifiSmartlink:Fail to Enter At Mode\r\n");
#endif
			return 1;
		}
		
		WIFI_HF_LPT200_Clear_RecvData();
		sprintf((char*)buff,"AT+WSSSID=%s\r\n",ssid);
		WIFI_HF_LPT200_Send_data(buff);
		WIFI_HF_LPT200_Check_data();
		WIFI_HF_LPT200_Clear_RecvData();
		delay_ms(20);
		
		st_memset((s8*)buff,0,64);
		sprintf((char*)buff,"AT+WSKEY=WPA2PSK,AES,%s\r\n",passwd);
		WIFI_HF_LPT200_Send_data(buff);
		WIFI_HF_LPT200_Check_data();
		WIFI_HF_LPT200_Clear_RecvData();
		
		//重启模块
		WIFI_HF_LPT200_Send_data((u8*)"AT+Z\r\n");
		WIFI_HF_LPT200_Check_data();
		WIFI_HF_LPT200_Clear_RecvData();
		
		for(i = 0;i < 10;i++)
			delay_ms(1000);
		
		delay_ms(400);
		WIFI_HF_LPT200_Enter_AT_Mode();
		
		if(WIFI_HF_LPT200_Check_NetWork_Status())
		{
			WIFI_HF_LPT200_Clear_RecvData();
#ifdef USART1_DEBUG
			printf("Wifi Set Fild!!!\r\n");
#endif
			return 1;
		}
		else
		{
			WIFI_HF_LPT200_Clear_RecvData();
			
			//蜂鸣器叫三声
			Beep_Contrl(BEEP_ON);
			delay_ms(200);
			Beep_Contrl(BEEP_OFF);
			delay_ms(200);
			Beep_Contrl(BEEP_ON);
			delay_ms(200);
			Beep_Contrl(BEEP_OFF);
			delay_ms(200);
			Beep_Contrl(BEEP_ON);
			delay_ms(200);
			Beep_Contrl(BEEP_OFF);
			
			return 0;
		}
	}
	
	return 0;
}



/****************************************************************
**函数功能：WIFI模块初始化
*****************************************************************/
void WIFI_HF_LPT200_Init(void)
{
	u8 ret = 0;
	u32 CPU_ID[3]  = {0};
	u32 val = 0;
	u8 wifi_config_params[60] = {0};
	
	ret = WIFI_HF_LPT200_Enter_AT_Mode();
	if(ret)
	{
		WIFI_HF_LPT200_Clear_RecvData();
		WIFI_HF_LPT200_Send_data((u8*)"AT+E\r\n");
		if(!WIFI_HF_LPT200_Check_data())
		{
			delay_ms(20);
			WIFI_HF_LPT200_Clear_RecvData();
		}
	}
	
	if(!Get_ChipID(CPU_ID))
	{
		srand(WIFI_UserNum_Val(CPU_ID));
		val = rand();
		
		sprintf((char*)wifi_config_params,"AT+WAP=11BGN,Family-Medipac-fm%05d,AUTO\r\n",val);
		
		WIFI_HF_LPT200_Send_data(wifi_config_params);
	}
	else
	{
		st_strcpy((s8*)wifi_config_params,"AT+WAP=11BGN,Family-Medipac,AUTO\r\n");
		
		WIFI_HF_LPT200_Send_data(wifi_config_params);
	}
	
	WIFI_HF_LPT200_Check_data();
	delay_ms(20);
	WIFI_HF_LPT200_Clear_RecvData();
	
	Set_HF_LPT200_Work_mode(WIFI_MODE_STA);
//	Set_HF_LPT200_Work_mode(WIFI_MODE_APSTA);//设置WIFI模块为APSTA模式工作
	WIFI_HF_LPT200_Check_data();
	delay_ms(20);
	WIFI_HF_LPT200_Clear_RecvData();
}


/**********************************************************
**函数功能：WIFI 引脚接口定义，初始化
**HF_GPIO14--------------PB15

**HF_RESET---------------PC2
**HF_nReady--------------PC13
**HF_nRelaod-------------PB9
**HF_nLink---------------PB8
***********************************************************/
void WIFI_HF_LPT200_Port_GPIO_Init(void)
{
	//定义一个GPIO_InitTypeDef类型的结构体
	GPIO_InitTypeDef GPIO_InitStructure;

	//开启GPIOE的外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE); 

	//HF_RESET
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	//HF_nRelaod
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	//HF_nReady
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//HF_nLink
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}


/**********************************************************
**函数功能：主MCU进入低功耗模式后，WIFI引脚接口的定义
**			STM32进入低功耗模式前，将必要引脚配置成浮空输入模式
***********************************************************/
void WIFI_HF_LPT200_PowerMode_Port_GPIO_ReInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*********************************************************
**函数功能：WIFI模块复位
**********************************************************/
void WIFI_HF_LPT200_ReSet(void)
{
	HF_ReSet(GPIO_OUT_LOW);
	delay_ms(15);
	HF_ReSet(GPIO_OUT_HIGH);
}

/**********************************************************
**函数功能：判断模块是否联网
**返回	值：0------连接成功
**			1------连接失败
***********************************************************/
u8 WIFI_HF_LPT200_Check_NetWorking(void)
{
	if(HF_nLink())
		return 1;
	else
		return 0;
}

/**********************************************************
**函数功能：判断模块是否启动成功
**返回	值：0-------启动成功
**			1-------启动失败
***********************************************************/
u8 WIFI_HF_LPT200_Check_Start(void)
{
	if(HF_nReady())
		return 1;
	else
		return 0;
}

/**********************************************************
**函数功能：恢复出厂设置
***********************************************************/
void WIFI_HF_LPT200_RestoreFactory(void)
{
	HF_nReload(GPIO_OUT_LOW);
	
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(500);
	
	HF_nReload(GPIO_OUT_HIGH);
}

/**********************************************************
**函数功能：WPS
***********************************************************/
#if 0
void WIFI_HF_LPT200_WPS(void)
{
	HF_GPIO14_WPS(GPIO_OUT_LOW);
	
	delay_ms(1000);
	
	HF_GPIO14_WPS(GPIO_OUT_HIGH);
}
#endif

#endif

