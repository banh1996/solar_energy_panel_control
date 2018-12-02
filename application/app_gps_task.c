#include "app_gps_task.h"

static TM_DELAY_Timer_t* 	timeout_timer;
static volatile bool		timeout_flag = false;
static char 				gps_data[100];
static char 				IP_SERVER[14] = "123.20.230.202";

static uint16_t usart_send_str(char *str)
{
    TM_USART_DMA_Send(USART1, (uint8_t *)str, strlen(str));
    while (TM_USART_DMA_Sending(USART1));
}

static uint16_t usart_get_str(char *str, uint16_t len)
{
	return TM_USART_Gets(USART1, str, len);
}

static void timeout_handler(void* UserParameters) 
{
	timeout_flag = true;
}

bool app_gps_request_and_get_reply(char *str_request,
							 	   char *str_reply_expect, 
								   uint16_t len_reply_expect)
{
	char receive_data_temp[200];
	memset(receive_data_temp, 0, 200);
	timeout_flag = false;
	TM_DELAY_TimerStart(timeout_timer);

	if(memcmp(str_request, "NULL", 4))
	{
		usart_send_str(str_request);
	}
	
	do
	{
		usart_get_str(receive_data_temp, sizeof(receive_data_temp));
	}
	while(memcmp(receive_data_temp, str_reply_expect, len_reply_expect) && !timeout_flag);
	TM_USART_ClearBuffer(USART1);
	TM_DELAY_TimerStop(timeout_timer);
	TM_DELAY_TimerReset(timeout_timer);


	// if(timeout_flag == false)
	// {
	// 	sprintf(&receive_data_temp[strlen(receive_data_temp)]," <=> %s\r\n", str_request);
	// 	TM_USART_Puts(USART2, receive_data_temp);
	// }
	// else
	// {
	// 	memcpy(receive_data_temp, str_request, strlen(str_request));
	// 	sprintf(&receive_data_temp[strlen(receive_data_temp)]," fail\r\n");
	// 	TM_USART_Puts(USART2, receive_data_temp);
	// }
	
	return !timeout_flag;
}

void app_gps_init(uint32_t baudrate_usart)
{
	/* Timer has reload value each 5s, enabled auto reload feature*/
	timeout_timer = TM_DELAY_TimerCreate(5000, 1, 0, timeout_handler, NULL);

	// TM_GPIO_Init(GPIOB, 
	// 			 GPIO_Pin_5, 
	// 			 TM_GPIO_Mode_OUT, 
	// 			 TM_GPIO_OType_PP, 
	// 			 TM_GPIO_PuPd_NOPULL, 
	// 			 TM_GPIO_Speed_High);

	// TM_GPIO_SetPinLow(GPIOB, GPIO_Pin_5);//PB5

	/* Init USART2 on pins TX = PA2, RX = PA3 */
	/* This pins are used on Nucleo boards for USB to UART via ST-Link */
	//TM_USART_Init(USART2, TM_USART_PinsPack_1, baudrate_usart);
	TM_USART_Init(USART1, TM_USART_PinsPack_2, baudrate_usart);//pb6 pb7
	/* Init TX DMA for USART1 */
	TM_USART_DMA_Init(USART1);
	
	//TM_USART_Puts(USART2, "test uart 2\r\n");

	// TM_GPIO_SetPinHigh(GPIOB, GPIO_Pin_5);//PB5

	while(!app_gps_request_and_get_reply("AT\r\n", "OK\r\n", 4));
	
	while(!app_gps_request_and_get_reply("AT+GPS=1\r\n", "OK\r\n", 4));
	
	usart_send_str("AT+CIPCLOSE\r\n");
}

void app_gps_get_value_and_send(char *str)
{
	char temp_str[100];
	
	memset(gps_data, 0, sizeof(gps_data));
	memset(temp_str, 0, sizeof(temp_str));
	
	app_gps_request_and_get_reply("AT+LOCATION=2\r\n", "AT+LOCATION=2\r\n", 15);

	timeout_flag = false;
	TM_DELAY_TimerStart(timeout_timer);
	do
	{
		usart_get_str(&temp_str[strlen(temp_str)], sizeof(temp_str) - strlen(temp_str));
	}
	while(memcmp(&temp_str[strlen(temp_str) - 4], "OK\r\n", 4) && !timeout_flag);

	TM_DELAY_TimerStop(timeout_timer);
	TM_DELAY_TimerReset(timeout_timer);
	TM_USART_ClearBuffer(USART1);

	if(!timeout_flag)
	{
		memcpy(gps_data, "GPS=", 4);
		memcpy(&gps_data[4], &temp_str[2], strlen(temp_str) - 8);

		//memcpy(str, gps_data, sizeof(gps_data));	

		if(app_gprs_send_data_to_sever(gps_data) == true)
		{
			//TM_USART_Puts(USART2, "send success\r\n");
		}
		else
		{
			//TM_USART_Puts(USART2, "send fail\r\n");
		}
	}
}

bool app_gprs_send_data_to_sever(char *str_send)
{	
	char temp_str[200];
	uint32_t len = 0xFE;
	memset(temp_str, 0, sizeof(temp_str));

	if(app_gps_request_and_get_reply("AT+CIPSTATUS=0\r\n", "+CIPSTATUS:0,CONNECT OK", 23) == false)
	{
		sprintf(temp_str, "AT+CIPSTART=\"TCP\",\"%s\",3000\r\n", IP_SERVER);	
		app_gps_request_and_get_reply(temp_str, "OK\r\n", 4);
	}
	
	Delayms(50);
	
	memset(temp_str, 0, sizeof(temp_str));
	usart_send_str("AT+CIPSEND\r\n");
	
	Delayms(50);
//	do
//	{
//		usart_get_str(&temp_str[strlen(temp_str)], sizeof(temp_str) - strlen(temp_str));
//	}
//	while(memcmp(&temp_str[strlen(temp_str) - 2], "> ", 2));

	usart_send_str("POST /api_send_location HTTP/1.1\r\n");

	memset(temp_str, 0, sizeof(temp_str));
	sprintf(temp_str, "Host: %s:3000\r\n", IP_SERVER);
	usart_send_str(temp_str);
	usart_send_str("Connection: keep-alive\r\n");

	memset(temp_str, 0, sizeof(temp_str));
	
	len = strlen(str_send);
	Delayms(50);
	sprintf(temp_str, "Content-Length: %d\r\n", len);
	
	usart_send_str(temp_str);
	
	usart_send_str("Accept: */*\r\n");

	memset(temp_str, 0, sizeof(temp_str));
	sprintf(temp_str, "Origin: http://%s:3000\r\n", IP_SERVER);
	usart_send_str(temp_str);
	usart_send_str("X-Requested-With: XMLHttpRequest\r\n");
	usart_send_str("User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.102 Safari/537.36\r\n");
	usart_send_str("Content-Type: application/x-www-form-urlencoded; charset=UTF-8\r\n");

	memset(temp_str, 0, sizeof(temp_str));
	sprintf(temp_str, "Referer: http://%s:3000/\r\n", IP_SERVER);
	usart_send_str(temp_str);
	usart_send_str("Accept-Encoding: gzip, deflate\r\n");
	usart_send_str("Accept-Language: vi-VN,vi;q=0.9,fr-FR;q=0.8,fr;q=0.7,en-US;q=0.6,en;q=0.5\r\n\r\n");

	memset(temp_str, 0, sizeof(temp_str));
	sprintf(temp_str, "%s%c", str_send, 0x1A);

	return app_gps_request_and_get_reply(temp_str, "Send successfully.", 18);
}
