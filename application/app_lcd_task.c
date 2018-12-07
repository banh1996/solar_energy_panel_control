#include "app_lcd_task.h"


void app_lcd_init(void)
{
	/* Initialize I2C2 */
	// pin PB10: SCL, PB11: SDA
	TM_I2C_Init(I2C2, TM_I2C_PinsPack_1, LCD_I2C_CLOCK);

	    	/* Check if device is connected */
	if (!TM_I2C_IsDeviceConnected(I2C2, LCD_I2C_ADDR)) 
  	{
		/* Return error */
		return MPU6050_DeviceNotConnected;
	}
}
