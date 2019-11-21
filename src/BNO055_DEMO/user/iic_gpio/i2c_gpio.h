#ifndef _I2C_GPIO_H_
#define _I2C_GPIO_H_

#include "stm32f10x.h" 
#include <inttypes.h>
#include "SysTick.h"

#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */

#if 0

void I2C_GPIOInitConfig(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendACK(u8 ack);
u8 I2C_RecvACK(void);
void I2C_SendByte(u8 dat);
u8 I2C_RecvByte(void);

uint8_t I2C_CheckDevice(uint8_t _Address);

#else


#define GPIO_PORT_I2C	GPIOB			/* GPIO端口 */
#define RCC_I2C_PORT 	RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define I2C_SCL_PIN		GPIO_Pin_6			/* 连接到SCL时钟线的GPIO */
#define I2C_SDA_PIN		GPIO_Pin_7			/* 连接到SDA数据线的GPIO */


#if 0	/* 条件编译： 1 选择GPIO的库函数实现IO读写 */
	#define I2C_SCL_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SCL_PIN)		/* SCL = 1 */
	#define I2C_SCL_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SCL_PIN)		/* SCL = 0 */
	
	#define I2C_SDA_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SDA_PIN)		/* SDA = 1 */
	#define I2C_SDA_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SDA_PIN)		/* SDA = 0 */
	
	#define I2C_SDA_READ()  GPIO_ReadInputDataBit(GPIO_PORT_I2C, I2C_SDA_PIN)	/* 读SDA口线状态 */
#else	/* 这个分支选择直接寄存器操作实现IO读写 */
    /*　注意：如下写法，在IAR最高级别优化时，会被编译器错误优化 */
	#define I2C_SCL_1()  GPIO_PORT_I2C->BSRR = I2C_SCL_PIN				/* SCL = 1 */
	#define I2C_SCL_0()  GPIO_PORT_I2C->BRR = I2C_SCL_PIN				/* SCL = 0 */
	
	#define I2C_SDA_1()  GPIO_PORT_I2C->BSRR = I2C_SDA_PIN				/* SDA = 1 */
	#define I2C_SDA_0()  GPIO_PORT_I2C->BRR = I2C_SDA_PIN				/* SDA = 0 */
	
	#define I2C_SDA_READ()  ((GPIO_PORT_I2C->IDR & I2C_SDA_PIN) != 0)	/* 读SDA口线状态 */
#endif

__inline void I2C_GPIOInitConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_I2C_PORT,ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStructure);

	GPIO_SetBits(GPIO_PORT_I2C, I2C_SCL_PIN | I2C_SDA_PIN);
}

 __inline static void SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStructure);

	GPIO_SetBits(GPIO_PORT_I2C,I2C_SDA_PIN);
}

 __inline static void SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStructure);

	GPIO_SetBits(GPIO_PORT_I2C,I2C_SDA_PIN);
}
/**************************************
**************************************/
 __inline void I2C_Start()
{
	SDA_OUT();
    I2C_SDA_1();
    I2C_SCL_1();       
    delay_us(100);     
    I2C_SDA_0(); 
    delay_us(100);   
    I2C_SCL_0();  
}

/**************************************
**************************************/
 __inline void I2C_Stop()
{
	SDA_OUT();
	I2C_SDA_0();
	I2C_SCL_1();  
	delay_us(100);
    I2C_SDA_1();
    delay_us(100);
}

/**************************************
**************************************/
__inline void I2C_SendACK(u8 ack)
{
	SDA_OUT();
	I2C_SCL_0();
	delay_us(100);
	if(ack)
		I2C_SDA_1();
	else I2C_SDA_0();
	I2C_SCL_1(); 
	delay_us(100);
	I2C_SCL_0();
	delay_us(100);
}

/**************************************
**************************************/
__inline u8 I2C_RecvACK(void)
{
	u8 ucErrTime=0;

 	SDA_IN();
	I2C_SDA_1();
	delay_us(100);	   
	I2C_SCL_1();
	delay_us(100);
		 
	while(I2C_SDA_READ())
	{
		ucErrTime++;
		delay_us(100);
		if(ucErrTime>250)
		{
			I2C_Stop();
			return 1;
		}
	}
	I2C_SCL_0();
	return 0;  
}

/**************************************
**************************************/
__inline void I2C_SendByte(u8 dat)
{
	u8 t; 
	SDA_OUT(); 	    
    I2C_SCL_0();
    for(t=0;t<8;t++)
    {
		if(dat&0x80)
			I2C_SDA_1();
		else I2C_SDA_0();
        dat<<=1; 	  
		delay_us(100);
		I2C_SCL_1();
		delay_us(100); 
		I2C_SCL_0();
		delay_us(100);
    }
}

/**************************************
**************************************/	

__inline u8 I2C_RecvByte(void)
{
	int i = 0;
	u8 byte = 0;
	SDA_IN();
	for(i = 0;i < 8;i++)
	{
		delay_us(100);
		I2C_SCL_1();
		delay_us(100);
		byte <<= 1;
		if(I2C_SDA_READ())
		{
			byte |= 0x01;
		}
		I2C_SCL_0();
		delay_us(100);
	}
	return byte;
}


/****************************************************************************
*	函 数 名: i2c_CheckDevice
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参：_Address：设备的I2C总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
****************************************************************************/
__inline uint8_t I2C_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;

	I2C_Start();		/* 发送启动信号 */

	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	I2C_SendByte(_Address | I2C_WR);
	delay_us(1000);
	ucAck = I2C_RecvACK();	/* 检测设备的ACK应答 */

	I2C_Stop();			/* 发送停止信号 */

	return ucAck;
}


#endif

#endif
