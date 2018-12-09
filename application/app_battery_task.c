#include "app_battery_task.h"

static uint16_t raw_data = 0;
static uint32_t real_m_volt = 0;
static uint32_t V_DROP = 134;

static uint8_t change_mvolt_to_battery(uint32_t raw_data_m_volt)
{
	if (raw_data_m_volt <= 10500)
	{
		return 0;
	}
	else if(raw_data_m_volt <= 11005)
	{
		return 5;
	}
	else if(raw_data_m_volt <= 11510)
	{
		return 10;
	}
	else if(raw_data_m_volt <= 11585)
	{
		return 15;
	}
	else if(raw_data_m_volt <= 11660)
	{
		return 20;
	}
	else if(raw_data_m_volt <= 11735)
	{
		return 25;
	}
	else if(raw_data_m_volt <= 11810)
	{
		return 30;
	}
	else if(raw_data_m_volt <= 11880)
	{
		return 35;
	}
	else if(raw_data_m_volt <= 11950)
	{
		return 40;
	}
	else if(raw_data_m_volt <= 12000)
	{
		return 45;
	}
	else if(raw_data_m_volt <= 12050)
	{
		return 50;
	}
	else if(raw_data_m_volt <= 12100)
	{
		return 55;
	}
	else if(raw_data_m_volt <= 12150)
	{
		return 60;
	}
	else if(raw_data_m_volt <= 12250)
	{
		return 65;
	}
	else if(raw_data_m_volt <= 12300)
	{
		return 70;
	}
	else if(raw_data_m_volt <= 12400)
	{
		return 75;
	}
	else if(raw_data_m_volt <= 12500)
	{
		return 80;
	}
	else if(raw_data_m_volt <= 12625)
	{
		return 85;
	}
	else if(raw_data_m_volt <= 12750)
	{
		return 90;
	}
	else if(raw_data_m_volt <= 12875)
	{
		return 95;
	}
	else
	{
		return 100;
	}
}

void app_battery_init(void)
{
	/* Initialize ADC2 on channel 14, this is pin PC4, Resolution 12bit */
	TM_ADC_Init(ADC2, ADC_Channel_14);
}

uint8_t app_battery_read(void)
{
	raw_data = TM_ADC_Read(ADC2, ADC_Channel_14);
	real_m_volt = (uint32_t)raw_data*125/14 - V_DROP;
	return change_mvolt_to_battery(real_m_volt);
}
