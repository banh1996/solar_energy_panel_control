#ifndef __GPS_TASK
#define __GPS_TASK

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "string.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_usart_dma.h"
#include "stm32f4xx_usart.h"

/**
 * @brief  A9G result enumeration	
 */
typedef enum 
{
	A9G_Ok = 0x00,          /*!< Everything OK */
	A9G_Device_Not_Available, /*!< There is no device with valid address */
	A9G_GPS_Error,     		/*!< Can't read GPS A9G */
	A9G_Not_Ready,			/*!< A9G module not ready */
	A9G_DeviceInvalid,       /*!< Connected device with address is not A9G */
	A9G_Send_Fail,
	A9G_Cant_connect_server
} A9G_Result_t;


/* brief: function for init GPS module
*/
A9G_Result_t app_gps_init(uint32_t baudrate_usart);

/* brief: function for get gps value at *str pointer
*/
A9G_Result_t app_gps_get_value_and_send(float speed,
										uint8_t battery_level,
										float temper);

#endif
