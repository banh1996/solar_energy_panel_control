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

static uint16_t adc0 = 0;
static uint16_t adc1 = 0;
static uint16_t adc2 = 0;
static uint16_t adc3 = 0;
static uint16_t num = 0;
static char str[100];

int main(void) 
{
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay functions */
	TM_DELAY_Init();
	
	//app_led_init();
	app_photoresistor_init();
	//app_led_on(LED_BLUE);
	app_motor_init(10);//10hz
	
	
	app_gps_init(115200);
	
	app_gps_get_value(str);
	 while(1)
	 {
			app_gps_get_value(str);
	 }

	Delayms(1111);
	 while (1) 
	 {
	 	adc0 = app_photoresistor_read(ADC_Channel_10);//PC0
	 	adc1 = app_photoresistor_read(ADC_Channel_11);//PC1
	 	adc2 = app_photoresistor_read(ADC_Channel_12);//PC2
	 	adc3 = app_photoresistor_read(ADC_Channel_13);//PC3
	 	Delayms(100);
	 }
}
