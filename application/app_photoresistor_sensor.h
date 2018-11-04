#ifndef __PHOTORESIS
#define __PHOTORESIS

#include "tm_stm32f4_adc.h"
#include <stdio.h>

/* brief: Initialize ADC3 on channel 0, this is pin PA10-PA11-PA12-PA13, Resolution 12bit
*/
void app_photoresistor_init(void);

/* brief: function for return 12bit adc
*/
uint16_t app_photoresistor_read(uint8_t channel);

#endif