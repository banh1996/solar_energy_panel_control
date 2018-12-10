#include "app_motor_control.h"

static bool 		motor1_flag = false;
static bool 		motor2_flag = false;

static TM_DELAY_Timer_t* 	duration_timer;

static TM_PWM_TIM_t TIM2_Data;
static TM_PWM_TIM_t TIM3_Data;

static uint16_t isforward_motor1 = 0;
static uint16_t	isforward_motor2 = 0;
static uint16_t adc0 = 0;
static uint16_t adc1 = 0;
static uint16_t adc2 = 0;
static uint16_t adc3 = 0;
static uint16_t adc_top = 0;
static uint16_t adc_bottom = 0;
static uint16_t adc_left = 0;
static uint16_t adc_right = 0;

static void off_motor_handler(void* UserParameters) 
{
	app_motor_stop(MOTOR1);
	app_motor_stop(MOTOR2);
}

void app_motor_init(uint16_t frequency)
{
	
	/* Set PWM to 50Hz frequency on timer TIM2 */
	/* 50Hz = 20ms = 20000us */
	TM_PWM_InitTimer(TIM2, &TIM2_Data, frequency);
	TM_PWM_InitTimer(TIM3, &TIM3_Data, frequency);
	
	/* Timer has reload value each 1s, disabled auto reload feature*/
	duration_timer = TM_DELAY_TimerCreate(1000, 0, 0, off_motor_handler, NULL);

	TM_GPIO_Init(GPIOA, 
				 GPIO_Pin_1 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, 
				 TM_GPIO_Mode_OUT, 
				 TM_GPIO_OType_PP, 	
				 TM_GPIO_PuPd_NOPULL, 
				 TM_GPIO_Speed_High);

	TM_GPIO_SetPinLow(GPIOA, GPIO_Pin_1);//PA1
	TM_GPIO_SetPinLow(GPIOA, GPIO_Pin_5);//PA5
	TM_GPIO_SetPinLow(GPIOA, GPIO_Pin_6);//PA6
	TM_GPIO_SetPinLow(GPIOA, GPIO_Pin_7);//PA7
	
	/* Initialize PWM on TIM2, TIM3, Channel 1 and PinsPack = PA5,PA6 */
	//TM_PWM_InitChannel(&TIM2_Data, TM_PWM_Channel_2, TM_PWM_PinsPack_1);//PA1
	//TM_PWM_InitChannel(&TIM2_Data, TM_PWM_Channel_1, TM_PWM_PinsPack_2);//PA5
	//TM_PWM_InitChannel(&TIM3_Data, TM_PWM_Channel_1, TM_PWM_PinsPack_1);//PA6
	//TM_PWM_InitChannel(&TIM3_Data, TM_PWM_Channel_2, TM_PWM_PinsPack_1);//PA7
}

void app_motor_start(uint8_t motor, uint16_t percent, bool isforward)
{
	TM_DELAY_TimerStart(duration_timer);
	if(motor == MOTOR1 && !motor1_flag)
	{
		if(isforward)
		{
			isforward_motor1 = 1;	
		}
		else
		{
			isforward_motor1 = 2;
		}
		if(isforward == true)
		{
			TM_GPIO_Init(GPIOA, GPIO_Pin_5, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High);
			TM_GPIO_SetPinLow(GPIOA, GPIO_Pin_5);//PA5
			TM_PWM_InitChannel(&TIM2_Data, TM_PWM_Channel_2, TM_PWM_PinsPack_1);//PA1
			Delayms(10);
			TM_PWM_SetChannelPercent(&TIM2_Data, TM_PWM_Channel_2, percent);								
		}
		else
		{
			TM_GPIO_Init(GPIOA, GPIO_Pin_1, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High);
			TM_GPIO_SetPinLow(GPIOA, GPIO_Pin_1);//PA1
			TM_PWM_InitChannel(&TIM2_Data, TM_PWM_Channel_1, TM_PWM_PinsPack_2);//PA5
			Delayms(10);
			TM_PWM_SetChannelPercent(&TIM2_Data, TM_PWM_Channel_1, percent);
		}		
		motor1_flag = true;
	}
	else if(motor == MOTOR2 && !motor2_flag)
	{
		if(isforward)
		{
			isforward_motor2 = 1;	
		}
		else
		{
			isforward_motor2 = 2;
		}
		if(isforward == true)
		{
			TM_GPIO_Init(GPIOA, GPIO_Pin_7, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High);
			TM_GPIO_SetPinLow(GPIOA, GPIO_Pin_7);//PA7
			TM_PWM_InitChannel(&TIM3_Data, TM_PWM_Channel_1, TM_PWM_PinsPack_1);//PA6
			Delayms(10);
			TM_PWM_SetChannelPercent(&TIM3_Data, TM_PWM_Channel_1, percent);			
		}
		else
		{
			TM_GPIO_Init(GPIOA, GPIO_Pin_6, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High);
			TM_GPIO_SetPinLow(GPIOA, GPIO_Pin_6);//PA6
			TM_PWM_InitChannel(&TIM3_Data, TM_PWM_Channel_2, TM_PWM_PinsPack_1);//PA7
			Delayms(10);
			TM_PWM_SetChannelPercent(&TIM3_Data, TM_PWM_Channel_2, percent);
		}	
		motor2_flag = true;
	}
}

void app_motor_stop(uint8_t motor)
{
	if(motor == MOTOR1 && motor1_flag)
	{
		if(isforward_motor1 == 1)
		{
			TM_PWM_SetChannelPercent(&TIM2_Data, TM_PWM_Channel_2, 0);//PA1
			TM_GPIO_Init(GPIOA, GPIO_Pin_1, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High);
			TM_GPIO_SetPinLow(GPIOA, GPIO_Pin_1);//PA1
			isforward_motor1 = false;
		}
		else if(isforward_motor1 == 2)
		{
			TM_PWM_SetChannelPercent(&TIM2_Data, TM_PWM_Channel_1, 0);//PA5
			TM_GPIO_Init(GPIOA, GPIO_Pin_5, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High);
			TM_GPIO_SetPinLow(GPIOA, GPIO_Pin_5);//PA5
		}
		isforward_motor1 = 0;
		motor1_flag = false;
	}
	else if(motor == MOTOR2 && motor2_flag)
	{
		if(isforward_motor2 == 1)
		{
			TM_PWM_SetChannelPercent(&TIM3_Data, TM_PWM_Channel_1, 0);//PA6
			TM_GPIO_Init(GPIOA, GPIO_Pin_6, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High);
			TM_GPIO_SetPinLow(GPIOA, GPIO_Pin_6);//PA6
			isforward_motor2 = false;
		}
		else if(isforward_motor2 == 2)
		{
			TM_PWM_SetChannelPercent(&TIM3_Data, TM_PWM_Channel_2, 0);//PA7
			TM_GPIO_Init(GPIOA, GPIO_Pin_7, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High);
			TM_GPIO_SetPinLow(GPIOA, GPIO_Pin_7);//PA7
		}
		isforward_motor2 = 0;
		motor2_flag = false;
	}
	Delayms(10);
}

void app_motor_control_servo(void)
{
	adc0 = app_photoresistor_read(ADC_Channel_10);
	adc1 = app_photoresistor_read(ADC_Channel_11);
	adc2 = app_photoresistor_read(ADC_Channel_12);
	adc3 = app_photoresistor_read(ADC_Channel_13);
	adc_top			= adc0/2 + adc3/2;
	adc_bottom 	= adc1/2 + adc2/2;
	adc_left 		= adc1/2 + adc3/2;
	adc_right 	= adc0/2 + adc2/2;

	if(abs(adc_top - adc_bottom) < STOP_THRESHOLD)
	{
		app_motor_stop(MOTOR1);
	}
	else
	{
		if(adc_top > adc_bottom)
		{
			if(!motor1_flag)
			{
				if(isforward_motor1 == 1)
				{
					app_motor_stop(MOTOR1);
					app_motor_start(MOTOR1, SPEED_PERCENT_1, false);
				}
				else if(isforward_motor1 == 0)
				{
					app_motor_start(MOTOR1, SPEED_PERCENT_1, false);
				}
			}
		}
		else
		{
			if(!motor1_flag)
			{
				if(isforward_motor1 == 2)
				{
					app_motor_stop(MOTOR1);
					app_motor_start(MOTOR1, SPEED_PERCENT_1, true);
				}
				else if(isforward_motor1 == 0)
				{
					app_motor_start(MOTOR1, SPEED_PERCENT_1, true);
				}
			}
		}
	}

	if(abs(adc_right - adc_left) < STOP_THRESHOLD)
	{
		app_motor_stop(MOTOR2);
	}
	else
	{
		if(adc_left < adc_right)
		{
			if(!motor2_flag)
			{
				if(isforward_motor2 == 1)
				{
					app_motor_stop(MOTOR2);
					app_motor_start(MOTOR2, SPEED_PERCENT_2, false);
				}
				else if(isforward_motor2 == 0)
				{
					app_motor_start(MOTOR2, SPEED_PERCENT_2, false);
				}
			}
		}
		else
		{
			if(!motor2_flag)
			{
				if(isforward_motor2 == 2)
				{
					app_motor_stop(MOTOR2);
					app_motor_start(MOTOR2, SPEED_PERCENT_2, true);
				}
				else if(isforward_motor2 == 0)
				{
					app_motor_start(MOTOR2, SPEED_PERCENT_2, true);
				}
			}
		}
	}
}
