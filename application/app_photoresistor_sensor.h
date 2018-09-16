#ifndef __PHOTORESIS
#define __PHOTORESIS

#include "tm_stm32f4_adc.h"
#include <stdio.h>

void app_photoresistor_init(void);
uint16_t app_photoresistor_read(uint8_t channel);

#endif