#include "app_photoresistor_sensor.h"

void app_photoresistor_init(void)
{
	/* Initialize ADC3 on channel 0, this is pin PA0-PA1-PA2-PA3, Resolution 12bit */
	TM_ADC_Init(ADC3, ADC_Channel_0);
	TM_ADC_Init(ADC3, ADC_Channel_1);
	TM_ADC_Init(ADC3, ADC_Channel_2);
	TM_ADC_Init(ADC3, ADC_Channel_3);
}

uint16_t app_photoresistor_read(uint8_t channel)
{
	return TM_ADC_Read(ADC1, channel);
}
