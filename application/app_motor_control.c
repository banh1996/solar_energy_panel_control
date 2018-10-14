#include "app_motor_control.h"

static bool 		motor1_flag = false;
static bool 		motor2_flag = false;
static TM_PWM_TIM_t TIM2_Data;
static TM_PWM_TIM_t TIM3_Data;

void app_motor_init(uint16_t frequency)
{
	
	/* Set PWM to 50Hz frequency on timer TIM2 */
	/* 50Hz = 20ms = 20000us */
	TM_PWM_InitTimer(TIM2, &TIM2_Data, frequency);
	TM_PWM_InitTimer(TIM3, &TIM3_Data, frequency);
	
	/* Initialize PWM on TIM2, TIM3, Channel 1 and PinsPack = PA5,PA6 */
	TM_PWM_InitChannel(&TIM2_Data, TM_PWM_Channel_1, TM_PWM_PinsPack_2);//PA5
	TM_PWM_InitChannel(&TIM3_Data, TM_PWM_Channel_1, TM_PWM_PinsPack_1);//PA6

	//init in input to reverse motor, pin PD9, PD10, PD11, PD12
	//PD9 motor1 forward, PD10 motor1 reverse, PD11 motor2 forward, PD12 motor 2 reverse
	TM_GPIO_Init(GPIOD,
				 GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12, 
				 TM_GPIO_Mode_OUT, 
				 TM_GPIO_OType_PP, 
				 TM_GPIO_PuPd_NOPULL, 
				 TM_GPIO_Speed_High);

	TM_GPIO_SetPinLow(GPIOD, GPIO_Pin_9);
	TM_GPIO_SetPinLow(GPIOD, GPIO_Pin_10);
	TM_GPIO_SetPinLow(GPIOD, GPIO_Pin_11);
	TM_GPIO_SetPinLow(GPIOD, GPIO_Pin_12);
}

void app_motor_start(uint8_t motor, uint16_t speed, bool isforward)
{
	if(motor == MOTOR1 && !motor1_flag)
	{
		if(isforward == true)
		{
			TM_GPIO_SetPinLow(GPIOD, GPIO_Pin_10);
			Delayms(10);
			TM_GPIO_SetPinHigh(GPIOD, GPIO_Pin_9);
		}
		else
		{
			TM_GPIO_SetPinLow(GPIOD, GPIO_Pin_9);
			Delayms(10);
			TM_GPIO_SetPinHigh(GPIOD, GPIO_Pin_10);
		}		
		TM_PWM_SetChannelMicros(&TIM2_Data, TM_PWM_Channel_1, speed);
		motor1_flag = true;
	}
	else if(motor == MOTOR2 && !motor2_flag)
	{
		if(isforward == true)
		{
			TM_GPIO_SetPinLow(GPIOD, GPIO_Pin_12);
			Delayms(10);
			TM_GPIO_SetPinHigh(GPIOD, GPIO_Pin_11);
		}
		else
		{
			TM_GPIO_SetPinLow(GPIOD, GPIO_Pin_11);
			Delayms(10);
			TM_GPIO_SetPinHigh(GPIOD, GPIO_Pin_12);
		}	
		TM_PWM_SetChannelMicros(&TIM3_Data, TM_PWM_Channel_1, speed);
		motor2_flag = true;
	}
}

void app_motor_stop(uint8_t motor)
{
	if(motor == MOTOR1 && motor1_flag)
	{
		TM_PWM_SetChannelMicros(&TIM2_Data, TM_PWM_Channel_1, 0);
		TM_GPIO_SetPinLow(GPIOD, GPIO_Pin_9);
		TM_GPIO_SetPinLow(GPIOD, GPIO_Pin_10);
		Delayms(10);
		motor1_flag = false;
	}
	else if(motor == MOTOR2 && motor2_flag)
	{
		TM_PWM_SetChannelMicros(&TIM3_Data, TM_PWM_Channel_1, 0);
		TM_GPIO_SetPinLow(GPIOD, GPIO_Pin_11);
		TM_GPIO_SetPinLow(GPIOD, GPIO_Pin_12);
		Delayms(10);
		motor2_flag = false;
	}
}