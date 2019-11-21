#ifndef _USART_DRIVER_H_
#define _USART_DRIVER_H_


#include <stdio.h>
#include "stm32f10x.h"

/**************************************************************************/
#define USART1_DEBUG
#define USART1_ENABLE
//#define USART3_ENABLE


#define USART3_BUFF	512

/**************************************************************************/
typedef enum
{
	USART1_BUF,
	USART2_BUF,
	USART3_BUF
}USART_Buf_Select;


/**************************************************************************/
typedef struct arg_USARTRecvData
{
	s32 Recvcount;
	s8 *Recv_start;
	s8 *Recv_end;
}USARTRecvData;


#ifdef USART2_ENABLE
extern s8 Usart2_buff[USART2_BUFF];
extern USARTRecvData Usart2_RecvStat;
#endif

#ifdef USART3_ENABLE
extern s8 Usart3_buff[USART3_BUFF];
extern USARTRecvData Usart3_RecvStat;
#endif

/**************************************************************************/


/***************************************
**USART1
****************************************/
void USART1_Config(void);
void UASRT_NVIC_Configuration(void);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);

char usart1_putc(char ch);
int usart1_putstr(unsigned char* buff,int len);
char usart1_getc(void);
int usart1_getstr(unsigned char* buff,int len);


/***************************************
**USART3
****************************************/
void USART3_Config(void);
char usart3_putc(char ch);
char usart3_getc(void);

int usart3_putstr(unsigned char* buff,int len);
int usart3_getstr(unsigned char* buff,int len);

int Usart3_Sendstr(unsigned char* buff);


/****************************************/
void Clear_USART_Buff(USART_Buf_Select usart);

#endif
