#ifndef __GPS_TASK
#define __GPS_TASK

#include <stdio.h>
#include <stdbool.h>
#include "string.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_usart_dma.h"

/* brief: function for init GPS module
*/
void app_gps_init(void);

/* brief: function for send data at *str pointer through usart1 PB6
*/
void usart_send_str(char *str, uint16_t len);

/* brief: function for receive data at *str pointer through usart1 PB7
*/
uint16_t usart_get_str(char *str, uint16_t len);

/* brief: function for get gps value at *str pointer
*/
void app_gps_get_value(char *str, uint16_t len);

#endif
