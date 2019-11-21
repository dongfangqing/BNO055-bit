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

#include "eeprom_driver.h"
#include "i2c_gpio.h"
#include "usart_driver.h"


#if 0

/* STM32 I2C 快速模式 */
#define I2C_Speed              100000

/* 这个地址只要与STM32外挂的I2C器件地址不一样即可 */
#define I2C1_OWN_ADDRESS7      0X0A   

/* AT24C01/02每页有8个字节 */
#define I2C_PageSize           8

/* AT24C04/08A/16A每页有16个字节 */
//#define I2C_PageSize           16			

uint16_t EEPROM_ADDRESS;

/**
  * @brief  I2C1 I/O配置
  * @param  无
  * @retval 无
  */
static void I2C_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 

	/* 使能与 I2C1 有关的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);  

	/* PB6-I2C1_SCL、PB7-I2C1_SDA*/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	      
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief  I2C 工作模式配置
  * @param  无
  * @retval 无
  */
static void I2C_Mode_Configu(void)
{
	I2C_InitTypeDef  I2C_InitStructure; 

	/* I2C 配置 */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;

	/* 高电平数据稳定，低电平数据变化 SCL 时钟线的占空比 */
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;

	I2C_InitStructure.I2C_OwnAddress1 =I2C1_OWN_ADDRESS7; 
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;

	/* I2C的寻址模式 */
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

	/* 通信速率 */
	I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;

	/* I2C1 初始化 */
	I2C_Init(I2C1, &I2C_InitStructure);

	/* 使能 I2C1 */
	I2C_Cmd(I2C1, ENABLE);   
}

/**
  * @brief  I2C 外设(EEPROM)初始化
  * @param  无
  * @retval 无
  */
void I2C_EE_Init(void)
{

	I2C_GPIO_Config(); 

	I2C_Mode_Configu();

	/* 根据头文件i2c_ee.h中的定义来选择EEPROM要写入的地址 */
#ifdef EEPROM_Block0_ADDRESS
	/* 选择 EEPROM Block0 来写入 */
	EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;
#endif

#ifdef EEPROM_Block1_ADDRESS  
	/* 选择 EEPROM Block1 来写入 */
	EEPROM_ADDRESS = EEPROM_Block1_ADDRESS;
#endif

#ifdef EEPROM_Block2_ADDRESS  
	/* 选择 EEPROM Block2 来写入 */
	EEPROM_ADDRESS = EEPROM_Block2_ADDRESS;
#endif

#ifdef EEPROM_Block3_ADDRESS  
	/* 选择 EEPROM Block3 来写入 */
	EEPROM_ADDRESS = EEPROM_Block3_ADDRESS;
#endif
}

/**
  * @brief   将缓冲区中的数据写到I2C EEPROM中
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  *     @arg NumByteToWrite:写的字节数
  * @retval  无
  */
void I2C_EE_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite)
{
	u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

	Addr = WriteAddr % I2C_PageSize;
	count = I2C_PageSize - Addr;
	NumOfPage =  NumByteToWrite / I2C_PageSize;
	NumOfSingle = NumByteToWrite % I2C_PageSize;

	/* If WriteAddr is I2C_PageSize aligned  */
	if(Addr == 0) 
	{
		/* If NumByteToWrite < I2C_PageSize */
		if(NumOfPage == 0) 
		{
			I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
			I2C_EE_WaitEepromStandbyState();
		}
		/* If NumByteToWrite > I2C_PageSize */
		else  
		{
			while(NumOfPage--)
			{
				I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize); 
				I2C_EE_WaitEepromStandbyState();
				WriteAddr +=  I2C_PageSize;
				pBuffer += I2C_PageSize;
			}

			if(NumOfSingle!=0)
			{
				I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
				I2C_EE_WaitEepromStandbyState();
			}
		}
	}
	/* If WriteAddr is not I2C_PageSize aligned  */
	else 
	{
		/* If NumByteToWrite < I2C_PageSize */
		if(NumOfPage== 0) 
		{
			I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
			I2C_EE_WaitEepromStandbyState();
		}
		/* If NumByteToWrite > I2C_PageSize */
		else
		{
			NumByteToWrite -= count;
			NumOfPage =  NumByteToWrite / I2C_PageSize;
			NumOfSingle = NumByteToWrite % I2C_PageSize;	

			if(count != 0)
			{  
				I2C_EE_PageWrite(pBuffer, WriteAddr, count);
				I2C_EE_WaitEepromStandbyState();
				WriteAddr += count;
				pBuffer += count;
			} 

			while(NumOfPage--)
			{
				I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize);
				I2C_EE_WaitEepromStandbyState();
				WriteAddr +=  I2C_PageSize;
				pBuffer += I2C_PageSize;  
			}
			if(NumOfSingle != 0)
			{
				I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle); 
				I2C_EE_WaitEepromStandbyState();
			}
		}
	}  
}

/**
  * @brief   写一个字节到I2C EEPROM中
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址 
  * @retval  无
  */
void I2C_EE_ByteWrite(u8* pBuffer, u8 WriteAddr)
{
	/* Send STRAT condition */
	I2C_GenerateSTART(I2C1, ENABLE);

	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));  

	/* Send EEPROM address for write */
	I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);

	/* Test on EV6 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/* Send the EEPROM's internal address to write to */
	I2C_SendData(I2C1, WriteAddr);

	/* Test on EV8 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	/* Send the byte to be written */
	I2C_SendData(I2C1, *pBuffer); 

	/* Test on EV8 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	/* Send STOP condition */
	I2C_GenerateSTOP(I2C1, ENABLE);
}

/**
  * @brief   在EEPROM的一个写循环中可以写多个字节，但一次写入的字节数
  *          不能超过EEPROM页的大小，AT24C02每页有8个字节
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  *     @arg NumByteToWrite:写的字节数
  * @retval  无
  */
void I2C_EE_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite)
{
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); // Added by Najoua 27/08/2008

	/* Send START condition */
	I2C_GenerateSTART(I2C1, ENABLE);

	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)); 

	/* Send EEPROM address for write */
	I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);

	/* Test on EV6 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));  

	/* Send the EEPROM's internal address to write to */    
	I2C_SendData(I2C1, WriteAddr);  

	/* Test on EV8 and clear it */
	while(! I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	/* While there is data to be written */
	while(NumByteToWrite--)  
	{
	/* Send the current byte */
	I2C_SendData(I2C1, *pBuffer); 

	/* Point to the next byte to be written */
	pBuffer++; 

	/* Test on EV8 and clear it */
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	}

	/* Send STOP condition */
	I2C_GenerateSTOP(I2C1, ENABLE);
}

/**
  * @brief   从EEPROM里面读取一块数据 
  * @param   
  *		@arg pBuffer:存放从EEPROM读取的数据的缓冲区指针
  *		@arg WriteAddr:接收数据的EEPROM的地址
  *     @arg NumByteToWrite:要从EEPROM读取的字节数
  * @retval  无
  */
void I2C_EE_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead)
{  
	//*((u8 *)0x4001080c) |=0x80; 
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); // Added by Najoua 27/08/2008    

	/* Send START condition */
	I2C_GenerateSTART(I2C1, ENABLE);
	//*((u8 *)0x4001080c) &=~0x80;

	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	/* Send EEPROM address for write */
	I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);

	/* Test on EV6 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/* Clear EV6 by setting again the PE bit */
	I2C_Cmd(I2C1, ENABLE);

	/* Send the EEPROM's internal address to write to */
	I2C_SendData(I2C1, ReadAddr);  

	/* Test on EV8 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	/* Send STRAT condition a second time */  
	I2C_GenerateSTART(I2C1, ENABLE);

	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	/* Send EEPROM address for read */
	I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Receiver);

	/* Test on EV6 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	/* While there is data to be read */
	while(NumByteToRead)  
	{
		if(NumByteToRead == 1)
		{
			/* Disable Acknowledgement */
			I2C_AcknowledgeConfig(I2C1, DISABLE);

			/* Send STOP Condition */
			I2C_GenerateSTOP(I2C1, ENABLE);
		}

		/* Test on EV7 and clear it */
		if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  
		{      
			/* Read a byte from the EEPROM */
			*pBuffer = I2C_ReceiveData(I2C1);

			/* Point to the next location where the byte read will be saved */
			pBuffer++; 

			/* Decrement the read bytes counter */
			NumByteToRead--;        
		}   
	}

	/* Enable Acknowledgement to be ready for another reception */
	I2C_AcknowledgeConfig(I2C1, ENABLE);
}

/**
  * @brief  Wait for EEPROM Standby state 
  * @param  无
  * @retval 无
  */
void I2C_EE_WaitEepromStandbyState(void)      
{
	vu16 SR1_Tmp = 0;

	do
	{
		/* Send START condition */
		I2C_GenerateSTART(I2C1, ENABLE);
		/* Read I2C1 SR1 register */
		SR1_Tmp = I2C_ReadRegister(I2C1, I2C_Register_SR1);
		/* Send EEPROM address for write */
		I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);
	}while(!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002));

	/* Clear AF flag */
	I2C_ClearFlag(I2C1, I2C_FLAG_AF);
	/* STOP condition */    
	I2C_GenerateSTOP(I2C1, ENABLE); 
}
#else

/*
*********************************************************************************************************
*	函 数 名: ee_CheckOk
*	功能说明: 判断串行EERPOM是否正常
*	形    参：无
*	返 回 值: 1 表示正常， 0 表示不正常
*********************************************************************************************************
*/
uint8_t ee_CheckOk(void)
{
	if (i2c_CheckDevice(EE_DEV_ADDR) == 0)
	{
		return 1;
	}
	else
	{
		/* 失败后，切记发送I2C总线停止信号 */
		i2c_Stop();		
		return 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: ee_ReadBytes
*	功能说明: 从串行EEPROM指定地址处开始读取若干数据
*	形    参：_usAddress : 起始地址
*			 _usSize : 数据长度，单位为字节
*			 _pReadBuf : 存放读到的数据的缓冲区指针
*	返 回 值: 0 表示失败，1表示成功
*********************************************************************************************************
*/
uint8_t ee_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i;
	
	/* 采用串行EEPROM随即读取指令序列，连续读取若干字节 */
	
	/* 第1步：发起I2C总线启动信号 */
	i2c_Start();
	
	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
//	i2c_SendByte(EE_DEV_ADDR | I2C_WR);	/* 此处是写指令 */
	i2c_SendByte(0XA0+((_usAddress/256)<<1));
	
	
	/* 第3步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
	i2c_SendByte((uint8_t)_usAddress%256);
	
	/* 第5步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	
	/* 第6步：重新启动I2C总线。前面的代码的目的向EEPROM传送地址，下面开始读取数据 */
	i2c_Start();
	
	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(EE_DEV_ADDR | I2C_RD);	/* 此处是读指令 */
	
	/* 第8步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}	
	
	/* 第9步：循环读取数据 */
	for (i = 0; i < _usSize; i++)
	{
		_pReadBuf[i] = i2c_ReadByte();	/* 读1个字节 */
		
		/* 每读完1个字节后，需要发送Ack， 最后一个字节不需要Ack，发Nack */
		if (i != _usSize - 1)
		{
			i2c_Ack();	/* 中间字节读完后，CPU产生ACK信号(驱动SDA = 0) */
		}
		else
		{
			i2c_NAck();	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
		}
	}
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 1;	/* 执行成功 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: ee_WriteBytes
*	功能说明: 向串行EEPROM指定地址写入若干数据，采用页写操作提高写入效率
*	形    参：_usAddress : 起始地址
*			 _usSize : 数据长度，单位为字节
*			 _pWriteBuf : 存放读到的数据的缓冲区指针
*	返 回 值: 0 表示失败，1表示成功
*********************************************************************************************************
*/
uint8_t ee_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i,m;
	uint16_t usAddr;
	
	/* 
		写串行EEPROM不像读操作可以连续读取很多字节，每次写操作只能在同一个page。
		对于24xx02，page size = 8
		简单的处理方法为：按字节写操作模式，没写1个字节，都发送地址
		为了提高连续写的效率: 本函数采用page wirte操作。
	*/

	usAddr = _usAddress;	
	for (i = 0; i < _usSize; i++)
	{
		/* 当发送第1个字节或是页面首地址时，需要重新发起启动信号和地址 */
		if ((i == 0) || (usAddr & (EE_PAGE_SIZE - 1)) == 0)
		{
			/*　第０步：发停止信号，启动内部写操作　*/
			i2c_Stop();
			
			/* 通过检查器件应答的方式，判断内部写操作是否完成, 一般小于 10ms 			
				CLK频率为200KHz时，查询次数为30次左右
			*/
			for (m = 0; m < 100; m++)
			{				
				/* 第1步：发起I2C总线启动信号 */
				i2c_Start();
				
				/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
//				i2c_SendByte(EE_DEV_ADDR | I2C_WR);	/* 此处是写指令 */
				i2c_SendByte(0XA0+((_usAddress/256)<<1));
				
				/* 第3步：发送一个时钟，判断器件是否正确应答 */
				if (i2c_WaitAck() == 0)
				{
					break;
				}
			}
			if (m  == 1000)
			{
				goto cmd_fail;	/* EEPROM器件写超时 */
			}
		
			/* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
			i2c_SendByte((uint8_t)usAddr%256);
			
			/* 第5步：发送ACK */
			if (i2c_WaitAck() != 0)
			{
				goto cmd_fail;	/* EEPROM器件无应答 */
			}
		}
	
		/* 第6步：开始写入数据 */
		i2c_SendByte(_pWriteBuf[i]);
	
		/* 第7步：发送ACK */
		if (i2c_WaitAck() != 0)
		{
			goto cmd_fail;	/* EEPROM器件无应答 */
		}

		usAddr++;	/* 地址增1 */		
	}
	
	/* 命令执行成功，发送I2C总线停止信号 */
	i2c_Stop();
	return 1;

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 0;
}

void ee_Erase(void)
{
	uint16_t i;
	uint8_t buf[EE_SIZE];
	
	/* 填充缓冲区 */
	for (i = 0; i < EE_SIZE; i++)
	{
		buf[i] = 0xFF;
	}

	/* 写EEPROM, 起始地址 = 0，数据长度为 256 */
	if (ee_WriteBytes(buf, 0, EE_SIZE) == 0)
	{
		printf("擦除eeprom出错！\r\n");
		return;
	}
	else
	{
		printf("擦除eeprom成功！\r\n");
	}
}


/*--------------------------------------------------------------------------------------------------*/

static void ee_Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

uint8_t write_buf[EE_SIZE];
uint8_t read_buf[EE_SIZE];
/*
 * eeprom AT24C02 读写测试
 */
void ee_Test(void)
{
	uint16_t i;
	

	printf("+++++++\r\n");
	
	if(ee_CheckOk() == 0)
	{
		/* 没有检测到EEPROM */
		printf("没有检测到串行EEPROM!\r\n");

		while (1);	/* 停机 */
	}
	printf("----------\r\n");
  
	/* 填充测试缓冲区 */
	for (i = 0; i < EE_SIZE; i++)
	{		
		write_buf[i] = i;
	}
	printf("----------\r\n");
  
	if (ee_WriteBytes(write_buf, 0, EE_SIZE) == 0)
	{
		printf("写eeprom出错！\r\n");
		return;
	}
	else
	{		
		printf("写eeprom成功！\r\n");
	}

	/*写完之后需要适当的延时再去读，不然会出错*/
	ee_Delay(0x0FFFFF);
	/*-----------------------------------------------------------------------------------*/
	if (ee_ReadBytes(read_buf, 0, EE_SIZE) == 0)
	{
		printf("读eeprom出错！\r\n");
		return;
	}
	else
	{
		printf("读eeprom成功，数据如下：\r\n");
	}
  
	for (i = 0; i < EE_SIZE; i++)
	{
//		if(read_buf[i] != write_buf[i])
//		{
//			printf("0x%02X ", read_buf[i]);
//			printf("错误:EEPROM读出与写入的数据不一致\r\n");
//			return;
//		}
		printf(" %02X", read_buf[i]);

		if ((i & 15) == 15)
		{
			printf("\r\n");	
		}
	}
	printf("eeprom读写测试成功\r\n");
	while(1);
}

#endif
