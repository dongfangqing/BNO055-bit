/*********************************SW-IOT32*********************************                                      
 
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
#include "sys_rcc.h"
#include "SysTick.h"
#include "led_driver.h" 
#include "usart_driver.h" 
#include "bno055.h"
#include "i2c_gpio.h"
#if 0
#include "beep_driver.h"
#include "spi_driver.h"
#include "OLED_Device.h"
#include "dht11_driver.h"
#include "ir_sensor.h"
#include "adc_driver.h"
#include "rtc_driver.h"
#include "display.h"
#include "key_driver.h"
#include "ST_string.h"
#include "eeprom_driver.h"
#endif

extern s32 bno055_data_readout_template(void);
extern uint8_t BNO055_CheckOk(void);

#if 0

extern struct bno055_t bno055;
extern s8 BNO055_I2C_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt);
int main(void)
{
	u8 chip_id = 0;
	u8 power_mode = BNO055_INIT_VALUE;
	
	RCC_Configuration();
	SysTick_init(72);
	USART1_Config();
	printf("hello main!!\r\n");
	LED_GPIO_Config();
	I2C_GPIOInitConfig();
	
	PWM_Channel1_Out(10);
	PWM_Channel2_Out(130);
	PWM_Channel3_Out(10);
	printf("hello ccc\r\n");

	if(!BNO055_CheckOk())
	{
		printf("I2C Bus No addrs: %x BNO055 device!!\r\n",BNO055_I2C_ADDR2);
//		while(1);
	}
	
	printf("+++++++++++++\r\n");
	bno055_data_readout_template();
//	BNO055_I2C_bus_read(BNO055_I2C_ADDR2,
//					BNO055_CHIP_ID_REG, &chip_id,BNO055_GEN_READ_WRITE_LENGTH);
	
	bno055_read_chip_id(&chip_id);
	
	printf("chip_id = 0x%x\r\n",chip_id);
	chip_id = 0;
	
	printf("==============\r\n");
	
	while(1)
	{
		delay_ms(1000);
		
		power_mode = BNO055_POWER_MODE_NORMAL;
	/* set the power mode as NORMAL*/
		bno055_set_power_mode(power_mode);

		bno055_set_operation_mode(BNO055_OPERATION_MODE_AMG);
		
		BNO055_I2C_bus_read(BNO055_I2C_ADDR2,
					BNO055_CHIP_ID_REG, &chip_id,BNO055_GEN_READ_WRITE_LENGTH);
		
//		bno055_read_chip_id(&chip_id);
	
		printf("chip_id = 0x%x\r\n",chip_id);
		chip_id = 0;

		#if 0
		BNO055_I2C_bus_read(BNO055_I2C_ADDR2,
						BNO055_ACCEL_REV_ID_ADDR, &chip_id,BNO055_GEN_READ_WRITE_LENGTH);
		printf("acc_id = 0x%x\r\n",chip_id);
		chip_id = 0;
		#endif
//		delay_ms(1000);
	}
}


#else
extern struct bno055_t bno055;
int main(void)
{
//	uint8_t e_buf[10] = {0};
//	uint8_t e_Rbuf[10] = {0};
//	uint8_t i = 0;
	/* variable used to read the accel x data */
	s16 accel_datax = BNO055_INIT_VALUE;
	 /* variable used to read the accel y data */
	s16 accel_datay = BNO055_INIT_VALUE;
	/* variable used to read the accel z data */
	s16 accel_dataz = BNO055_INIT_VALUE;
	/* variable used to read the mag x data */
	s16 mag_datax  = BNO055_INIT_VALUE;
	/* variable used to read the mag y data */
	s16 mag_datay  = BNO055_INIT_VALUE;
	/* variable used to read the mag z data */
	s16 mag_dataz  = BNO055_INIT_VALUE;
	/* variable used to read the gyro x data */
	s16 gyro_datax = BNO055_INIT_VALUE;
	/* variable used to read the gyro y data */
	s16 gyro_datay = BNO055_INIT_VALUE;
	 /* variable used to read the gyro z data */
	s16 gyro_dataz = BNO055_INIT_VALUE;
	s32 comres = BNO055_ERROR;

	/* variable used to read the euler h data */
	s16 euler_data_h = BNO055_INIT_VALUE;
	 /* variable used to read the euler r data */
	s16 euler_data_r = BNO055_INIT_VALUE;
	/* variable used to read the euler p data */
	s16 euler_data_p = BNO055_INIT_VALUE;

	unsigned char accel_calib_status = 0;
	unsigned char gyro_calib_status = 0;
	unsigned char mag_calib_status = 0;
	unsigned char sys_calib_status = 0;
	
	u8 chip_id = 0;
	u8 ret = 0;
	
	RCC_Configuration();
	SysTick_init(72);
	USART1_Config();
//	SPI_InitConfig();
	printf("hello main!!\r\n");
	LED_GPIO_Config();
	I2C_GPIOInitConfig();
	
	PWM_Channel1_Out(10);
	PWM_Channel2_Out(130);
	PWM_Channel3_Out(10);
//	printf("hello main!!\r\n");
	printf("hello ccc\r\n");

	if(!BNO055_CheckOk())
	{
		printf("I2C Bus No addrs: %x BNO055 device!!\r\n",BNO055_I2C_ADDR2);
//		while(1);
	}
	
	printf("+++++++++++++\r\n");
	bno055_data_readout_template();
	
	bno055_read_chip_id(&chip_id);
	
	printf("chip_id = %d\r\n",chip_id);
	
	printf("==============\r\n");
	
	
	while(1)
	{
		delay_ms(1000);

		bno055_set_power_mode(BNO055_POWER_MODE_NORMAL);
		bno055_set_operation_mode(BNO055_OPERATION_MODE_AMG);
//		
#if 1
		comres += bno055_read_accel_x(&accel_datax);
		comres += bno055_read_accel_y(&accel_datay);
		comres += bno055_read_accel_z(&accel_dataz);
		
		printf("accel Data: comres = %d\r\n",comres);
		printf("accel_datax = %d\r\n",accel_datax);
		printf("accel_datay = %d\r\n",accel_datay);
		printf("accel_dataz = %d\r\n",accel_dataz);
		printf(" \r\n");
#endif
#if 1		
		comres += bno055_read_mag_x(&mag_datax);
		comres += bno055_read_mag_y(&mag_datay);
		comres += bno055_read_mag_z(&mag_dataz);
		printf("mag Data: comres = %d\r\n",comres);
		printf("mag_datax = %d\r\n",mag_datax);
		printf("mag_datay = %d\r\n",mag_datay);
		printf("mag_dataz = %d\r\n",mag_dataz);
		printf(" \r\n");
#endif
#if 1		
		comres += bno055_read_gyro_x(&gyro_datax);
		comres += bno055_read_gyro_y(&gyro_datay);
		comres += bno055_read_gyro_z(&gyro_dataz);
		printf("gyro Data: comres = %d\r\n",comres);
		printf("gyro_datax = %d\r\n",gyro_datax);
		printf("gyro_datay = %d\r\n",gyro_datay);
		printf("gyro_dataz = %d\r\n",gyro_dataz);
		printf(" \r\n");
#endif

#if 0
//		bno055_set_power_mode(BNO055_POWER_MODE_NORMAL);
		bno055_set_operation_mode(BNO055_OPERATION_MODE_NDOF);
		comres += bno055_read_euler_h(&euler_data_h);
		comres += bno055_read_euler_r(&euler_data_r);
		comres += bno055_read_euler_p(&euler_data_p);
		
		printf("accel Data: comres = %d\r\n",comres);
		printf("euler_data_h = %d\r\n",euler_data_h);
		printf("euler_data_r = %d\r\n",euler_data_r);
		printf("euler_data_p = %d\r\n",euler_data_p);
		printf(" \r\n");
		
#endif
		delay_ms(1000);
		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
		
		
		bno055_read_chip_id(&chip_id);
	
		printf("chip_id = %d\r\n",chip_id);

		comres = 0;
	}
}
#endif

