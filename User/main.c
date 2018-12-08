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
#include "app_motor_control.h"
#include "app_photoresistor_sensor.h"
#include "app_led_control.h"
#include "app_gps_task.h"
#include "app_mpu6050_task.h"
#include "app_lcd_task.h"

static uint16_t adc0 = 0;
static uint16_t adc1 = 0;
static uint16_t adc2 = 0;
static uint16_t adc3 = 0;
static uint16_t num = 0;
static MPU6050_data_t MPU6050_data;
static char str[100];

int main(void) 
{
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay functions */
	TM_DELAY_Init();
	
	//app_led_init();
	//app_photoresistor_init();
	//app_led_on(LED_BLUE);
	//app_motor_init(10);//10hz	
	//app_gps_init(115200);
	TM_USART_Init(USART2, TM_USART_PinsPack_1, 115200);
	TM_USART_Puts(USART2, "test uart 2\r\n");
	if (app_mpu_6050_init(MPU6050_Accelerometer_2G, MPU6050_Gyroscope_250s) != MPU6050_Ok) 
	{
		TM_USART_Puts(USART2, "MPU6050 Error\n");
		while (1);
	}

	
	if(app_lcd_init() != LCD_Ok)
	{
		TM_USART_Puts(USART2, "LCD Error\n");
		while (1);
	}

	app_lcd_send_string("banh\r\n");
	
	while (1) 
	{
		/* Read all data from sensor */
		app_mpu6050_ReadAll(&MPU6050_data);
		
		/* Format data */		
		sprintf(str, "Accelerometer\n- X:%0.3f\n- Y:%0.3f\n- Z:%0.3f\nGyroscope\n- X:%0.3f\n- Y:%0.3f\n- Z:%0.3f\nTemperature\n- %3.4f\n\n\n",
			MPU6050_data.Accelerometer_X,
			MPU6050_data.Accelerometer_Y,
			MPU6050_data.Accelerometer_Z,
			MPU6050_data.Gyroscope_X,
			MPU6050_data.Gyroscope_Y,
			MPU6050_data.Gyroscope_Z,
			MPU6050_data.Temperature
		);
		
		/* Show to usart */
		TM_USART_Puts(USART2, str);
		
		/* Little delay */
		Delayms(500);
	}

	while(1)
	{
	app_gps_get_value_and_send(str);
	//app_gprs_send_data();
		Delayms(3000);
	}
	
	
	Delayms(1111);
	while (1) 
	{
		app_gps_get_value_and_send(str);
//	 	adc0 = app_photoresistor_read(ADC_Channel_10);//PC0
//	 	adc1 = app_photoresistor_read(ADC_Channel_11);//PC1
//	 	adc2 = app_photoresistor_read(ADC_Channel_12);//PC2
//	 	adc3 = app_photoresistor_read(ADC_Channel_13);//PC3
		
//		app_motor_control_servo(app_photoresistor_read(ADC_Channel_10),
//														 app_photoresistor_read(ADC_Channel_11),
//														 app_photoresistor_read(ADC_Channel_12),
//														 app_photoresistor_read(ADC_Channel_13));
	//app_motor_control_servo(adc0, adc1, adc2, adc3);
		Delayms(20);
	}
}
