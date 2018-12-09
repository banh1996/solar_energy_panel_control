#include "app_led_control.h"

static bool red_flag   = false;
static bool green_flag = false;
static bool blue_flag  = false;
static bool orange_flag  = false;
static bool init_flag  = false;

void app_led_init(void)
{
	if(!init_flag)
	{
		TM_DISCO_LedInit();
		init_flag = true;
	}
}

void app_led_on(uint16_t pin)
{
	if(init_flag)
	{
		if(pin == LED_RED && !red_flag)
		{
			TM_DISCO_LedOn(pin);
			red_flag = true;
		}
		if(pin == LED_GREEN && !green_flag)
		{
			TM_DISCO_LedOn(pin);
			green_flag = true;
		}
		if(pin == LED_BLUE && !blue_flag)
		{
			TM_DISCO_LedOn(pin);
			blue_flag = true;
		}
		if(pin == LED_ORANGE && !orange_flag)
		{
			TM_DISCO_LedOn(pin);
			blue_flag = true;
		}
	}
}

void app_led_off(uint16_t pin)
{
	if(init_flag)
	{
		if(pin == LED_RED && red_flag)
		{
			TM_DISCO_LedOff(pin);
			red_flag = false;
		}
		if(pin == LED_GREEN && green_flag)
		{
			TM_DISCO_LedOff(pin);
			green_flag = false;
		}
		if(pin == LED_BLUE && blue_flag)
		{
			TM_DISCO_LedOff(pin);
			blue_flag = false;
		}
		if(pin == LED_ORANGE && orange_flag)
		{
			TM_DISCO_LedOff(pin);
			blue_flag = false;
		}
	}
}

void app_led_toggle(uint16_t pin)
{
	if(init_flag)
	{
		if(pin == LED_RED)
		{
			TM_DISCO_LedToggle(pin);
			red_flag = !red_flag;
		}
		if(pin == LED_GREEN)
		{
			TM_DISCO_LedToggle(pin);
			green_flag = !green_flag;
		}
		if(pin == LED_BLUE)
		{
			TM_DISCO_LedToggle(pin);
			blue_flag = !blue_flag;
		}
		if(pin == LED_ORANGE)
		{
			TM_DISCO_LedToggle(pin);
			orange_flag = !orange_flag;
		}
	}
}
