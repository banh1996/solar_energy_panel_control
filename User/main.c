/**
 *	Keil project for PWM SERVO example
 *
 *	Servo works with 50Hz (20ms) PWM frequency input.
 *	High pulse is between 1 and 2 ms, but some are working from 0.8 to 2.2ms
 *	My at home is going from 0.9 to 2.1 ms
 *
 *	Before you start, select your target, on the right of the "Load" button
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "tm_stm32f4_pwm.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_watchdog.h"
#include "app_motor_control.h"
#include "app_photoresistor_sensor.h"
#include "app_led_control.h"
#include "app_gps_task.h"
#include "app_mpu6050_task.h"
#include "app_lcd_task.h"
#include "app_battery_task.h"

static MPU6050_data_t	MPU6050_data;
static char 		  	str[100];
//static char				str_MPU[100];
static uint8_t 			level_battery = 0;

int main(void) 
{
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay functions */
	TM_DELAY_Init();
	
	/* Disable watchdog when we are in debug mode */
	DBGMCU->APB1FZ |= DBGMCU_IWDG_STOP;
	
	app_motor_init(10);//10hz	
	app_photoresistor_init();
	app_battery_init();

	TM_USART_Init(USART2, TM_USART_PinsPack_1, 9600);//pa2, pa3
	TM_USART_Puts(USART2, "test uart 2\r\n");

	app_gps_init(9600);


	if (app_mpu_6050_init(MPU6050_Accelerometer_2G, MPU6050_Gyroscope_250s) != MPU6050_Ok) 
	{
		TM_USART_Puts(USART2, "MPU6050 Error\n");
	}

	app_battery_read(&level_battery);

	if(app_lcd_init() != LCD_Ok)
	{
		TM_USART_Puts(USART2, "LCD Error\n");
	}
	else
	{
		lcd_send_cmd(0x01);
		sprintf(str, "59-V2 __ 497.09     Battery level: %d", level_battery);
		str[strlen(str)] = '%';
		Delayms(800);
		app_lcd_send_string(str);
		Delayms(900);
	}
	
	/* Initialize watchdog timer */
	/* Set timeout to 32s */
	/* If we are in debug mode, watchdog won't start even if we enable it */
	TM_WATCHDOG_Init(TM_WATCHDOG_Timeout_32s);
	
	while (1) 
	{
		/* Read all data from sensor */
		app_mpu6050_ReadAll(&MPU6050_data);
		//MPU6050_data.Temperature;//calib
		/* Format data */		
		// sprintf(str_MPU, 
		// 		"Accelerometer\n- X:%0.3f\n- Y:%0.3f\n- Z:%0.3f\nGyroscope\n- X:%0.3f\n- Y:%0.3f\n- Z:%0.3f\nTemperature\n- %3.4f\n\n\n",
		// 		MPU6050_data.Accelerometer_X,
		// 		MPU6050_data.Accelerometer_Y,
		// 		MPU6050_data.Accelerometer_Z,
		// 		MPU6050_data.Gyroscope_X,
		// 		MPU6050_data.Gyroscope_Y,
		// 		MPU6050_data.Gyroscope_Z,
		// 		MPU6050_data.Temperature);
		// TM_USART_Puts(USART2, str_MPU);

		if(app_battery_read(&level_battery) == true)
		{
			lcd_send_cmd(0x01);
			sprintf(str, "59-V2 __ 497.09     Battery level: %d", level_battery);
			str[strlen(str)] = '%';
			Delayms(20);
			app_lcd_send_string(str);
		}

		app_gps_get_value_and_send(0.0, 
								   level_battery,
								   MPU6050_data.Temperature);

		app_motor_control_servo();
								
		/* Little delay */
		TM_WATCHDOG_Reset();
	}
}
