#ifndef _KEY_DRIVER_H_
#define _KEY_DRIVER_H_



#include "stm32f10x.h"


typedef enum 
{
	NO_KEY = 0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4
}KEY_PORT_STATUS;



void Key_GPIOConfig(void);
uint8_t key_scan(void);


#endif

