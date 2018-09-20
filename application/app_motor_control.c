#include "app_motor_control.h"

void app_motor_init(void)
{
	TM_PWM_TIM_t TIM2_Data;
	/* Set PWM to 50Hz frequency on timer TIM2 */
	/* 50Hz = 20ms = 20000us */
	TM_PWM_InitTimer(TIM2, &TIM2_Data, 50);
	
	/* Initialize PWM on TIM2, Channel 1 and PinsPack 2 = PA5 */
	TM_PWM_InitChannel(&TIM2_Data, TM_PWM_Channel_1, TM_PWM_PinsPack_2);
	
	/* Set channel 1 value, 1500us = servo at center position */
	TM_PWM_SetChannelMicros(&TIM2_Data, TM_PWM_Channel_1, 1500);
}