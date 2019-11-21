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
#include "spi_driver.h"
#include "usart_driver.h"

/**********************************************************
**************************引脚定义*************************
**SPI_NSS:			PA4
**SPI_CLK:			PA5
**SPI_MISO:			PA6
**SPI_MOSI:			PA7
***********************************************************/

/*********************************************************
					变量定义
*********************************************************/



/******************************************************
**函数功能：SPI初始化
*******************************************************/
void SPI_InitConfig(void)
{
	#if 0
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	

	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx; 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	 			
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	 	
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		 	
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;		
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		  
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  			
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);

	// Enable SPI1  
	SPI_Cmd(SPI1, ENABLE);
	#endif
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
//	printf("SPI_InitConfig(void)\r\n");
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
}




#if 0
void SPI_WriterByte(u8 dat)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	// 通过 SPI2发送一字节数据 
	SPI_I2S_SendData(SPI1, dat);	
}
#endif

#if 1
#define OLED_SCLK_Clr()		GPIO_ResetBits(GPIOA, GPIO_Pin_5)
#define OLED_SCLK_Set()		GPIO_SetBits(GPIOA, GPIO_Pin_5)

#define OLED_SDIN_Clr()		GPIO_ResetBits(GPIOA, GPIO_Pin_7)	
#define OLED_SDIN_Set()		GPIO_SetBits(GPIOA, GPIO_Pin_7)


void SPI_WriterByte(u8 dat)
{	
	u8 i;			  

	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		{
			OLED_SDIN_Set();
		}
		else
			OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}
}
#endif

