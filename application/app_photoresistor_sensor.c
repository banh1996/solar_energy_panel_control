#include "app_photoresistor_sensor.h"

void app_photoresistor_init(void)
{
	/* Initialize ADC2, this is pin PC0-PC1-PC2-PC3-PC5, Resolution 12bit */
	TM_ADC_Init(ADC2, ADC_Channel_10);
	TM_ADC_Init(ADC2, ADC_Channel_11);
	TM_ADC_Init(ADC2, ADC_Channel_12);
	TM_ADC_Init(ADC2, ADC_Channel_13);
	TM_ADC_Init(ADC2, ADC_Channel_15);
}

uint16_t app_photoresistor_read(uint8_t channel)
{
	return TM_ADC_Read(ADC2, channel);
}
