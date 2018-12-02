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

/* brief: function for init GPS module
*/
void app_gps_init(uint32_t baudrate_usart);

/* brief: function for get gps value at *str pointer
*/
void app_gps_get_value_and_send(char *str);

bool app_gps_request_and_get_reply(char *str_request,
							 	   char *str_reply_expect, uint16_t len_reply_expect);

bool app_gprs_send_data_to_sever(char *str_send);

#endif
