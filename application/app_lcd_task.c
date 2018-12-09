#include "app_lcd_task.h"

void lcd_send_cmd (char cmd)
{
  	char data_u, data_l;
	uint8_t data_temp[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_temp[0] = data_u|0x0C;  //en=1, rs=0
	data_temp[1] = data_u|0x08;  //en=0, rs=0
	data_temp[2] = data_l|0x0C;  //en=1, rs=0
	data_temp[3] = data_l|0x08;  //en=0, rs=0
	TM_I2C_WriteMultiNoRegister(I2C2, 
					  			LCD_SLAVE_I2C_ADDR, 
					  			(uint8_t *)data_temp, 
					  			4);
}

static void lcd_send_data (char data)
{
	char data_u, data_l;
	uint8_t data_temp[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_temp[0] = data_u|0x0D;  //en=1, rs=0
	data_temp[1] = data_u|0x09;  //en=0, rs=0
	data_temp[2] = data_l|0x0D;  //en=1, rs=0
	data_temp[3] = data_l|0x09;  //en=0, rs=0
	TM_I2C_WriteMultiNoRegister(I2C2, 
								LCD_SLAVE_I2C_ADDR, 
								(uint8_t *)data_temp, 
								4);
}

LCD_Result_t app_lcd_init(void)
{
	/* Initialize I2C2 */
	// pin PB10: SCL, PB11: SDA
	TM_I2C_Init(I2C2, TM_I2C_PinsPack_1, LCD_I2C_CLOCK);

	    	/* Check if device is connected */
	if (!TM_I2C_IsDeviceConnected(I2C2, LCD_SLAVE_I2C_ADDR))
    {
		/* Return error */
		return LCD_DeviceNotConnected;
	}

	lcd_send_cmd(0x02);
	lcd_send_cmd(0x28);
	lcd_send_cmd(0x0C);
	lcd_send_cmd(0x80);
	return LCD_Ok;
}

LCD_Result_t app_lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
	return LCD_Ok;
}

LCD_Result_t app_lcd_set_cursor (uint8_t x, uint8_t y)
{
	lcd_send_cmd(0x0c+x);
	lcd_send_cmd(0x80+y);
	return LCD_Ok;
}
