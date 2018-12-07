#ifndef __LED_CONTROL
#define __LED_CONTROL

#include "tm_stm32f4_disco.h"
#include <stdio.h>
#include <stdbool.h>

/* brief: init led board
*/
void app_led_init(void);

/** brief: on led
	*	param: @pin LED_RED, LED_GREEN, LED_BLUE, LED_ORANGE
*/
void app_led_on(uint16_t pin);

/** brief: off led
	*	param: @pin LED_RED, LED_GREEN, LED_BLUE, LED_ORANGE
*/
void app_led_off(uint16_t pin);

/** brief: toggle led
	*	param: @pin LED_RED, LED_GREEN, LED_BLUE, LED_ORANGE
*/
void app_led_toggle(uint16_t pin);

#endif