#include "app_gps_task.h"

static TM_DELAY_Timer_t* 	timeout_timer;
static volatile bool		timeout_flag = false;
static char 				gps_data[200];
static char 				IP_SERVER[14] = "113.173.228.93";
static char					g_receiver_data[200];
static bool					gb_waiting_receiver = false;
static A9G_state_t			g_GPS_state_global = A9G_State_Waiting;
static A9G_Result_t			g_result_GPS;

static void usart_send_str(char *str)
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

static A9G_Result_t app_gps_request_and_get_reply(char *str_request,
												  char *str_reply_expect, 
												  uint16_t len_reply_expect)
{
	if(gb_waiting_receiver == true)
	{
		if((memcmp(&g_receiver_data[strlen(g_receiver_data) - len_reply_expect], str_reply_expect, len_reply_expect) && !timeout_flag) == false)
		{
			TM_USART_ClearBuffer(USART1);
			gb_waiting_receiver = false;
			if(timeout_flag)
			{
				return A9G_Receive_Not_Ok;//receive not ok
			}
			else
			{
				TM_DELAY_TimerStop(timeout_timer);
				TM_DELAY_TimerReset(timeout_timer);
				return A9G_Ok;//receive ok
			}
		}		
		else
		{
			usart_get_str(&g_receiver_data[strlen(g_receiver_data)], sizeof(g_receiver_data) - strlen(g_receiver_data));
			return A9G_Waiting_reply;//waiting
		}
	}
	else
	{
		if(memcmp(str_request, "NULL", 4))
		{
			usart_send_str(str_request);	
		}
		TM_USART_ClearBuffer(USART1);
		memset(g_receiver_data, 0, sizeof(g_receiver_data));
		TM_DELAY_TimerStart(timeout_timer);
		timeout_flag = false;
		gb_waiting_receiver = true;
		return A9G_Send_Already;//sended already
	}
}





A9G_Result_t app_gps_init(uint32_t baudrate_usart)
{
	/* Timer has reload value each 8s, disabled auto reload feature*/
	timeout_timer = TM_DELAY_TimerCreate(2000, 0, 0, timeout_handler, NULL);

	/* Init USART1 on pins TX = PB6, RX = PB7 */
	/* This pins are used on Nucleo boards for USB to UART via ST-Link */
	TM_USART_Init(USART1, TM_USART_PinsPack_2, baudrate_usart);
	/* Init TX DMA for USART1 */
	TM_USART_DMA_Init(USART1);

	usart_send_str("AT+RST=1\r\n");//reset
	Delayms(10000);

	TM_USART_ClearBuffer(USART1);
	
	while(app_gps_request_and_get_reply("AT\r\n", "OK\r\n", 4) != A9G_Ok)
	;

	return A9G_Ok;
}

A9G_Result_t app_gps_get_value_and_send(float speed,
										uint8_t battery_level,
										float temper)
{
	switch(g_GPS_state_global)
	{
		case A9G_State_Waiting:
		{
			g_GPS_state_global = A9G_State_Send_Comand_GPS;
			return A9G_Ok;
		}
		case A9G_State_Send_Comand_GPS:
		{
			g_result_GPS = app_gps_request_and_get_reply("AT+GPS=1\r\n", "OK\r\n", 4);
			if(g_result_GPS == A9G_Ok)
			{
				g_GPS_state_global = A9G_State_Send_Comand_Location;
				return A9G_Ok;
			}
			break;
		}
		case A9G_State_Send_Comand_Location:
		{
			g_result_GPS = app_gps_request_and_get_reply("AT+LOCATION=2\r\n", "OK\r\n", 4);
			if(g_result_GPS == A9G_Ok)
			{	
				g_GPS_state_global = A9G_State_Getting_GPS;
				return A9G_Ok;
			}
			break;
		}
		case A9G_State_Getting_GPS:
		{
			memcpy(gps_data, "Id=1&GPS=", 10);
			memcpy(&gps_data[strlen(gps_data)], &g_receiver_data[18], strlen(g_receiver_data) - 8);
			sprintf(&gps_data[strlen(gps_data)], "&Speed=%0.2f", speed);
			sprintf(&gps_data[strlen(gps_data)], "&Capacity=%d", battery_level);
			sprintf(&gps_data[strlen(gps_data)], "&Temp=%0.1f", temper);
			g_GPS_state_global = A9G_State_Checking_Server_Connection;
			return A9G_Ok;
		}
		case A9G_State_Checking_Server_Connection:
		{
			g_result_GPS = app_gps_request_and_get_reply("AT+CIPSTATUS=0\r\n", "+CIPSTATUS:0,IP INITIAL", 23);
			if(g_result_GPS == A9G_Ok)
			{
				g_GPS_state_global = A9G_State_Request_Connect;
				return A9G_Ok;
			}
			g_result_GPS = app_gps_request_and_get_reply("AT+CIPSTATUS=0\r\n", "+CIPSTATUS:0,CONNECT OK", 23);
			if(g_result_GPS == A9G_Ok)
			{
				g_GPS_state_global = A9G_State_Start_Sending_GPS;
				return A9G_Ok;
			}
			if(g_result_GPS == A9G_Receive_Not_Ok)
			{
				g_GPS_state_global = A9G_State_Request_Reset;
				return A9G_Receive_Not_Ok;
			}
			break;
		}
		case A9G_State_Request_Reset:
		{
			usart_send_str("AT+RST=1\r\n");
			g_GPS_state_global = A9G_State_Waiting;
			break;
		}
		case A9G_State_Request_Connect:
		{
			g_result_GPS = app_gps_request_and_get_reply("AT+CIPSTART=\"TCP\",\"%s\",3000\r\n", "OK\r\n", 4);
			if(g_result_GPS == A9G_Ok)
			{
				g_GPS_state_global = A9G_State_Start_Sending_GPS;
			}
			else if(g_result_GPS == A9G_Receive_Not_Ok)
			{
				g_GPS_state_global = A9G_State_Request_Reset;
			}
			break;
		}
		case A9G_State_Start_Sending_GPS:
		{
			g_result_GPS = app_gps_request_and_get_reply("AT+CIPSEND\r\n", "> ", 4);
			if(g_result_GPS == A9G_Ok)
			{
				char temp_str[100];
				uint32_t len_data_send = strlen(gps_data);
				memset(temp_str, 0, sizeof(temp_str));
				usart_send_str("POST /api_send_location HTTP/1.1\r\n");
				sprintf(temp_str, "Host: %s:3000\r\n", IP_SERVER);
				usart_send_str(temp_str);
				usart_send_str("Connection: keep-alive\r\n");

				memset(temp_str, 0, sizeof(temp_str));
				sprintf(temp_str, "Content-Length: %d\r\n", len_data_send);
				
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
				sprintf(temp_str, "%s%c", gps_data, 0x1A);
				Delayms(100);
				g_GPS_state_global = A9G_State_Waiting_Reply_Server;
				return A9G_Ok;
			}
			break;
		}
		case A9G_State_Waiting_Reply_Server:
		{
			g_result_GPS = app_gps_request_and_get_reply("NULL", "Send successfully.\r\n", 20);
			if(g_result_GPS == A9G_Ok)
			{
				memset(gps_data, 0, sizeof(gps_data));
				g_GPS_state_global = A9G_State_Send_Comand_GPS;
			}
			else if(g_result_GPS == A9G_Receive_Not_Ok)
			{
				memset(gps_data, 0, sizeof(gps_data));
				return A9G_Send_Fail;
			}
			break;
		}
		default:
			break;
	}
	return g_result_GPS;










	// if(app_gps_request_and_get_reply("AT+GPS=1\r\n", "OK\r\n", 4) == A9G_Ok)
	// {
	// 	return A9G_GPS_Error;
	// }

	// if(app_gps_request_and_get_reply("AT+LOCATION=2\r\n", "AT+LOCATION=2\r\n", 15) == false)
	// {
	// 	return A9G_GPS_Error;
	// }

	// timeout_flag = false;
	// TM_DELAY_TimerStart(timeout_timer);
	// do
	// {
	// 	usart_get_str(&temp_str[strlen(temp_str)], sizeof(temp_str) - strlen(temp_str));
	// }
	// while(memcmp(&temp_str[strlen(temp_str) - 4], "OK\r\n", 4) && !timeout_flag);

	// TM_DELAY_TimerStop(timeout_timer);
	// TM_DELAY_TimerReset(timeout_timer);
	// TM_USART_ClearBuffer(USART1);

	// if(timeout_flag == true)
	// {
	// 	return A9G_GPS_Error;
	// }
	// else
	// {
	// 	memcpy(gps_data, "Id=1&GPS=", 10);
	// 	memcpy(&gps_data[strlen(gps_data)], &temp_str[2], strlen(temp_str) - 8);
	// 	sprintf(&gps_data[strlen(gps_data)], "&Speed=%0.2f", speed);
	// 	sprintf(&gps_data[strlen(gps_data)], "&Capacity=%d", battery_level);
	// 	sprintf(&gps_data[strlen(gps_data)], "&Temp=%0.1f", temper);

	// 	TM_USART_ClearBuffer(USART1);
		
	// 	return app_gprs_send_data_to_sever(gps_data);
	// }
}

// static A9G_Result_t app_gprs_send_data_to_sever(char *str_send)
// {
// 	char temp_str[100];
// 	uint32_t len_data_send = strlen(str_send);

// 	uint8_t temp_check = app_gps_wait_connect_server();
// 	if(temp_check == 0)
// 	{
// 		usart_send_str("AT+RST=1\r\n");
// 		return A9G_Cant_connect_server;
// 	}
// 	else if(temp_check == 2)
// 	{
// 		memset(temp_str, 0, sizeof(temp_str));
// 		sprintf(temp_str, "AT+CIPSTART=\"TCP\",\"%s\",3000\r\n", IP_SERVER);	
// 		if(app_gps_request_and_get_reply(temp_str, "OK\r\n", 4) == A9G_Ok)
// 		{
// 			usart_send_str("AT+RST=1\r\n");
// 			return A9G_Cant_connect_server;
// 		}
// 	}
	
// 	memset(temp_str, 0, sizeof(temp_str));
// 	usart_send_str("AT+CIPSEND\r\n");
	
// 	Delayms(100);

// 	usart_send_str("POST /api_send_location HTTP/1.1\r\n");

// 	memset(temp_str, 0, sizeof(temp_str));
// 	sprintf(temp_str, "Host: %s:3000\r\n", IP_SERVER);
// 	usart_send_str(temp_str);
// 	usart_send_str("Connection: keep-alive\r\n");

// 	memset(temp_str, 0, sizeof(temp_str));
// 	sprintf(temp_str, "Content-Length: %d\r\n", len_data_send);
	
// 	usart_send_str(temp_str);
	
// 	usart_send_str("Accept: */*\r\n");

// 	memset(temp_str, 0, sizeof(temp_str));
// 	sprintf(temp_str, "Origin: http://%s:3000\r\n", IP_SERVER);
// 	usart_send_str(temp_str);
// 	usart_send_str("X-Requested-With: XMLHttpRequest\r\n");
// 	usart_send_str("User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.102 Safari/537.36\r\n");
// 	usart_send_str("Content-Type: application/x-www-form-urlencoded; charset=UTF-8\r\n");

// 	memset(temp_str, 0, sizeof(temp_str));
// 	sprintf(temp_str, "Referer: http://%s:3000/\r\n", IP_SERVER);
// 	usart_send_str(temp_str);
// 	usart_send_str("Accept-Encoding: gzip, deflate\r\n");
// 	usart_send_str("Accept-Language: vi-VN,vi;q=0.9,fr-FR;q=0.8,fr;q=0.7,en-US;q=0.6,en;q=0.5\r\n\r\n");

// 	memset(temp_str, 0, sizeof(temp_str));
// 	sprintf(temp_str, "%s%c", str_send, 0x1A);
// 	Delayms(100);

// 	if(app_gps_request_and_get_reply(temp_str, "Send successfully.\r\n", 20) == true)
// 	{
// 		return A9G_Ok;
// 	}
// 	else
// 	{
// 		return A9G_Send_Fail;
// 	}
// }

// static uint8_t app_gps_wait_connect_server(void)
// {
// 	char receive_data_temp[100];

// 	usart_send_str("AT+CIPSTATUS=0\r\n");

// 	memset(receive_data_temp, 0, sizeof(receive_data_temp));
// 	timeout_flag = false;
// 	TM_DELAY_TimerStart(timeout_timer);	
	
// 	do
// 	{
// 		usart_get_str(receive_data_temp, sizeof(receive_data_temp));
// 	}
// 	while(memcmp(receive_data_temp, "+CIPSTATUS:0,CONNECT OK", 23) &&
// 		  memcmp(receive_data_temp, "+CIPSTATUS:0,IP INITIAL", 23) &&
// 		  !timeout_flag);

// 	TM_DELAY_TimerStop(timeout_timer);
// 	TM_DELAY_TimerReset(timeout_timer);
// 	TM_USART_ClearBuffer(USART1);

// 	if(timeout_flag)
// 	{
// 		return 0;
// 	}
// 	else if(memcmp(receive_data_temp, "+CIPSTATUS:0,CONNECT OK", 23) == 0)
// 	{
// 		return 1;
// 	}
// 	else if(memcmp(receive_data_temp, "+CIPSTATUS:0,IP INITIAL", 23) == 0)
// 	{
// 		return 2;
// 	}
// 	return 0;
// }
