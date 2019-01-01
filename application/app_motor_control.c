#include "app_motor_control.h"

static TM_PWM_TIM_t TIM2_Data;
static TM_PWM_TIM_t TIM3_Data;

static uint16_t g_state_motor1 = 0;
static uint16_t	g_state_motor2 = 0;
static uint16_t adc0 = 0;
static uint16_t adc1 = 0;
static uint16_t adc2 = 0;
static uint16_t adc3 = 0;
static uint16_t adc_top = 0;
static uint16_t adc_bottom = 0;
static uint16_t adc_left = 0;
static uint16_t adc_right = 0;
static uint16_t adc_behind = 0;

void app_motor_init(uint16_t frequency)
{
	TM_PWM_InitTimer(TIM2, &TIM2_Data, frequency);
	TM_PWM_InitTimer(TIM3, &TIM3_Data, frequency);
	
	/* Initialize PWM on TIM2, TIM3, Channel 1 and PinsPack = PA5,PA6 */
	TM_PWM_InitChannel(&TIM2_Data, TM_PWM_Channel_2, TM_PWM_PinsPack_1);//PA1
	TM_PWM_InitChannel(&TIM2_Data, TM_PWM_Channel_1, TM_PWM_PinsPack_2);//PA5
	TM_PWM_InitChannel(&TIM3_Data, TM_PWM_Channel_1, TM_PWM_PinsPack_1);//PA6
	TM_PWM_InitChannel(&TIM3_Data, TM_PWM_Channel_2, TM_PWM_PinsPack_1);//PA7
	TM_PWM_SetChannelPercent(&TIM2_Data, TM_PWM_Channel_1, 0);
	TM_PWM_SetChannelPercent(&TIM2_Data, TM_PWM_Channel_2, 0);
	TM_PWM_SetChannelPercent(&TIM3_Data, TM_PWM_Channel_1, 0);
	TM_PWM_SetChannelPercent(&TIM3_Data, TM_PWM_Channel_2, 0);
}

void app_motor_start(uint8_t motor, uint16_t percent, bool isforward)
{
	//TM_DELAY_TimerStart(duration_timer);
	if(motor == MOTOR1 && g_state_motor1 == 0)
	{
		if(isforward == true)
		{
			TM_PWM_SetChannelPercent(&TIM2_Data, TM_PWM_Channel_1, 0);//set PA5 -> GND	
			Delayms(5);
			TM_PWM_SetChannelPercent(&TIM2_Data, TM_PWM_Channel_2, percent);//on PA1			
			g_state_motor1 = 1;						
		}
		else
		{
			TM_PWM_SetChannelPercent(&TIM2_Data, TM_PWM_Channel_2, 0);//set PA1 -> GND	
			Delayms(5);
			TM_PWM_SetChannelPercent(&TIM2_Data, TM_PWM_Channel_1, percent);//on PA5
			g_state_motor1 = 2;
		}		
	}
	else if(motor == MOTOR2 && g_state_motor2 == 0)
	{
		if(isforward == true)
		{
			TM_PWM_SetChannelPercent(&TIM3_Data, TM_PWM_Channel_2, 0);	//set PA7 -> GND	
			Delayms(5);
			TM_PWM_SetChannelPercent(&TIM3_Data, TM_PWM_Channel_1, percent);//on PA6	
			g_state_motor2 = 1;			
		}
		else
		{
			TM_PWM_SetChannelPercent(&TIM3_Data, TM_PWM_Channel_1, 0); //set PA6 -> GND
			Delayms(5);
			TM_PWM_SetChannelPercent(&TIM3_Data, TM_PWM_Channel_2, percent);//PA7
			g_state_motor2 = 2;
		}	
	}
}

void app_motor_stop(uint8_t motor)
{
	if(motor == MOTOR1)
	{
		if(g_state_motor1 == 1)
		{
			TM_PWM_SetChannelPercent(&TIM2_Data, TM_PWM_Channel_2, 0);//PA1
		}
		else if(g_state_motor1 == 2)
		{
			TM_PWM_SetChannelPercent(&TIM2_Data, TM_PWM_Channel_1, 0);//PA5
		}
		g_state_motor1 = 0;
	}
	else if(motor == MOTOR2)
	{
		if(g_state_motor2 == 1)
		{
			TM_PWM_SetChannelPercent(&TIM3_Data, TM_PWM_Channel_1, 0);//PA6
		}
		else if(g_state_motor2 == 2)
		{
			TM_PWM_SetChannelPercent(&TIM3_Data, TM_PWM_Channel_2, 0);//PA7
		}
		g_state_motor2 = 0;
	}
}

void app_motor_control_servo(void)
{
	adc0 = app_photoresistor_read(ADC_Channel_10);
	adc1 = app_photoresistor_read(ADC_Channel_11);
	adc2 = app_photoresistor_read(ADC_Channel_12);
	adc3 = app_photoresistor_read(ADC_Channel_13);
	adc_behind = app_photoresistor_read(ADC_Channel_15);

	if(adc0 > 3500 && adc1 > 3500 && adc2> 3500 && adc3 > 3500 && adc_behind < 2900)
	{
		app_motor_start(MOTOR1, SPEED_PERCENT_1, false);
	}
	else if(adc0 > 3500 && adc1 > 3500 && adc2> 3500 && adc3 > 3500 && adc_behind > 3500)
	{
		app_motor_stop(MOTOR1);
	}

	if(adc0 > 1600)
	{
		adc0 = adc0 - 70;
	}
	
	if(adc3 > 1600)
	{
		adc0 = adc0 - 20;
	}
	
	adc_top		= adc0/2 + adc3/2;
	adc_bottom 	= adc1/2 + adc2/2;
	adc_left 	= adc1/2 + adc3/2;
	adc_right 	= adc0/2 + adc2/2;

	if(abs(adc_top - adc_bottom) < STOP_THRESHOLD)
	{
		app_motor_stop(MOTOR1);
	}
	else
	{
		if(adc_top > adc_bottom)
		{
			if(g_state_motor1 == 1)
			{
				app_motor_stop(MOTOR1);
				app_motor_start(MOTOR1, SPEED_PERCENT_1, false);
			}
			else if(g_state_motor1 == 0)
			{
				app_motor_start(MOTOR1, SPEED_PERCENT_1, false);
			}
		}
		else
		{
			if(g_state_motor1 == 2)
			{
				app_motor_stop(MOTOR1);
				app_motor_start(MOTOR1, SPEED_PERCENT_1, true);
			}
			else if(g_state_motor1 == 0)
			{
				app_motor_start(MOTOR1, SPEED_PERCENT_1, true);
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
			if(g_state_motor2 == 1)
			{
				app_motor_stop(MOTOR2);
				app_motor_start(MOTOR2, SPEED_PERCENT_2, false);
			}
			else if(g_state_motor2 == 0)
			{
				app_motor_start(MOTOR2, SPEED_PERCENT_2, false);
			}
		}
		else
		{
			if(g_state_motor2 == 2)
			{
				app_motor_stop(MOTOR2);
				app_motor_start(MOTOR2, SPEED_PERCENT_2, true);
			}
			else if(g_state_motor2 == 0)
			{
				app_motor_start(MOTOR2, SPEED_PERCENT_2, true);
			}
		}
	}
}
