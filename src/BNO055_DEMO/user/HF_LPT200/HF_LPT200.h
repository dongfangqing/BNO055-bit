#ifndef _HF_LPT200_H_
#define _HF_LPT200_H_

#include "stm32f10x.h" 



/***************************************************************/
#define GPIO_OUT_HIGH	1
#define GPIO_OUT_LOW	0


#define HF_ReSet(a)		if (a)	\
						GPIO_SetBits(GPIOC,GPIO_Pin_2);\
						else		\
						GPIO_ResetBits(GPIOC,GPIO_Pin_2)
					
#define HF_nReload(a)	if (a)	\
						GPIO_SetBits(GPIOB,GPIO_Pin_9);\
						else		\
						GPIO_ResetBits(GPIOB,GPIO_Pin_9)
						
#define HF_nLink()	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)					

#define HF_nReady()	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)			

#if 0
#define HF_GPIO14_WPS(a)	if (a)	\
							GPIO_SetBits(GPIOB,GPIO_Pin_15);\
							else		\
							GPIO_ResetBits(GPIOB,GPIO_Pin_15)
#endif
						
/***************************************************************/

typedef enum{
	WIFI_MODE_AP,
	WIFI_MODE_STA,
	WIFI_MODE_APSTA
}WIFIWork_Mode;



/**************************************************************/
#if 0
void WIFI_HF_LPT200_Send_data(u8* str);
u8 WIFI_HF_LPT200_Read_data(void);
u8 WIFI_HF_LPT200_Check_data(void);
void WIFI_HF_LPT200_Clear_RecvData(void);
u8 WIFI_HF_LPT200_trying_In_Command_Mode(void);
u8 WIFI_HF_LPT200_Enter_AT_Mode(void);
u8 WIFI_HF_LPT200_To_ATMode(void);
u8 WIFI_HF_LPT200_Leave_ATMode(void);

u8 WIFI_HF_LPT200_Check_NetWork_Status(void);
void WIFI_HF_LPT200_Set_TCPClient_Mode(u8* IP_Addr);
void WIFI_HF_LPT200_Set_TCPServer_Mode(void);

void WIFI_HF_LPT200_Set_ID(u8* id);
void WIFI_HF_LPT200_Get_ID(u8* id);
void Set_HF_LPT200_Work_mode(WIFIWork_Mode mode);
u8 WIFI_HF_LPT200_Set_Args(u8* ssid,u8* passwd);




void WIFI_HF_LPT200_Init(void);
void WIFI_HF_LPT200_Port_GPIO_Init(void);
void WIFI_HF_LPT200_PowerMode_Port_GPIO_ReInit(void);
void WIFI_HF_LPT200_ReSet(void);
u8 WIFI_HF_LPT200_Check_NetWorking(void);
u8 WIFI_HF_LPT200_Check_Start(void);
void WIFI_HF_LPT200_RestoreFactory(void);
void WIFI_HF_LPT200_WPS(void);


void WIFI_HF_LPT200_Power_ON(void);
void WIFI_HF_LPT200_Power_OFF(void);

#endif
#endif
