#ifndef __BATTERY
#define __BATTERY

#include "tm_stm32f4_adc.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* brief: Initialize ADC3 on channel 0, this is pin PC0-PC1-PC2-PC3, Resolution 12bit
*/
void app_battery_init(void);

/* brief: function for return 12bit adc
*/
bool app_battery_read(uint8_t *bat_level);

#endif
