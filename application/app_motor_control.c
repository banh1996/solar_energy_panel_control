#include "app_motor_control.h"

static bool 		motor1_flag = false;
static bool 		motor2_flag = false;
static bool 		isforward_motor1 = false;
static bool 		isforward_motor2 = false;

static TM_PWM_TIM_t TIM2_Data;
static TM_PWM_TIM_t TIM3_Data;

void app_motor_init(uint16_t frequency)
{
	
	/* Set PWM to 50Hz frequency on timer TIM2 */
	/* 50Hz = 20ms = 20000us */
	TM_PWM_InitTimer(TIM2, &TIM2_Data, frequency);
	TM_PWM_InitTimer(TIM3, &TIM3_Data, frequency);
	
	/* Initialize PWM on TIM2, TIM3, Channel 1 and PinsPack = PA5,PA6 */
	//TM_PWM_InitChannel(&TIM2_Data, TM_PWM_Channel_2, TM_PWM_PinsPack_1);//PA1
	//TM_PWM_InitChannel(&TIM2_Data, TM_PWM_Channel_1, TM_PWM_PinsPack_2);//PA5
	//TM_PWM_InitChannel(&TIM3_Data, TM_PWM_Channel_1, TM_PWM_PinsPack_1);//PA6
	//TM_PWM_InitChannel(&TIM3_Data, TM_PWM_Channel_2, TM_PWM_PinsPack_1);//PA7
}

void app_motor_start(uint8_t motor, uint16_t percent, bool isforward)
{
	if(motor == MOTOR1 && !motor1_flag)
	{
		isforward_motor1 = isforward;	
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
		isforward_motor2 = isforward;
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
		if(isforward_motor1 == true)
		{
			TM_PWM_SetChannelPercent(&TIM2_Data, TM_PWM_Channel_2, 0);//PA1
			TM_GPIO_Init(GPIOA, GPIO_Pin_1, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High);
			TM_GPIO_SetPinLow(GPIOA, GPIO_Pin_1);//PA1
		}
		else
		{
			TM_PWM_SetChannelPercent(&TIM2_Data, TM_PWM_Channel_1, 0);//PA5
			TM_GPIO_Init(GPIOA, GPIO_Pin_5, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High);
			TM_GPIO_SetPinLow(GPIOA, GPIO_Pin_5);//PA5
		}
		motor1_flag = false;
	}
	else if(motor == MOTOR2 && motor2_flag)
	{
		if(isforward_motor2 == true)
		{
			TM_PWM_SetChannelPercent(&TIM3_Data, TM_PWM_Channel_1, 0);//PA6
			TM_GPIO_Init(GPIOA, GPIO_Pin_6, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High);
			TM_GPIO_SetPinLow(GPIOA, GPIO_Pin_6);//PA6
		}
		else
		{
			TM_PWM_SetChannelPercent(&TIM3_Data, TM_PWM_Channel_2, 0);//PA7
			TM_GPIO_Init(GPIOA, GPIO_Pin_7, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High);
			TM_GPIO_SetPinLow(GPIOA, GPIO_Pin_7);//PA7
		}
		motor2_flag = false;
	}
	Delayms(10);
}