/* brief: Project uses PCF8574 I2c
*/

#ifndef __LCD_TASK
#define __LCD_TASK

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "tm_stm32f4_i2c.h"
#include "tm_stm32f4_delay.h"

#define LCD_I2C_CLOCK	    100000
#define LCD_SLAVE_I2C_ADDR	0x4E

/**
 * @brief  PCF8574 result enumeration	
 */
typedef enum 
{
	LCD_Ok = 0x00,          /*!< Everything OK */
	LCD_DeviceNotConnected, /*!< There is no device with valid slave address */
	LCD_DeviceInvalid       /*!< Connected device with address is not MPU6050 */
} LCD_Result_t;


/* brief: init lcd module
*/
LCD_Result_t app_lcd_init(void);

/* brief: put string to lcd
*/
LCD_Result_t app_lcd_send_string (char *str);

LCD_Result_t app_lcd_set_cursor (uint8_t x, uint8_t y);

void lcd_send_cmd (char cmd);

#endif
