#include "app_led_control.h"

void app_led_init(void)
{
	TM_DISCO_LedInit();
}

void app_led_on(uint16_t pin)
{
	TM_DISCO_LedOn(pin);
}

void app_led_off(uint16_t pin)
{
	TM_DISCO_LedOff(pin);
}

void app_led_toggle(uint16_t pin)
{
	TM_DISCO_LedToggle(pin);
}