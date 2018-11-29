#include "app_gps_task.h"

static char gps_data[200];
static char IP_SERVER[14] = "123.20.230.202";
static bool gb_gps_data_ready = false;
/* Variables used */
static TM_DELAY_Timer_t* timeout_timer;

// init AT command here

static volatile bool	timeout_flag = false;

/* Called when transfer is completed for specific stream */
static void TM_DMA_TransferCompleteHandler(DMA_Stream_TypeDef* DMA_Stream) {
	/* Check if interrupt is for correct stream */
	if (DMA_Stream == TM_USART_DMA_GetStream(USART2)) {
		//TM_USART_Puts(USART2, "Stream has finished with transfer\n");
	}
}

/* Called when half transfer is completed for specific stream */
static void TM_DMA_HalfTransferCompleteHandler(DMA_Stream_TypeDef* DMA_Stream) {
	/* Check if interrupt is for correct stream */
	if (DMA_Stream == TM_USART_DMA_GetStream(USART2)) {
		/* Do stuff here */
		//TM_USART_Puts(USART2, "Stream has finished with half transfer\n");
	}
}

/* Called when transfer error occurs for specific stream */
static void TM_DMA_TransferErrorHandler(DMA_Stream_TypeDef* DMA_Stream) {
	/* Check if interrupt is for correct stream */
	if (DMA_Stream == TM_USART_DMA_GetStream(USART2)) {
		/* Do stuff here */
		//TM_USART_Puts(USART2, "Stream transfer error occured\n");
	}
}

/* Called when direct mode error occurs for specific stream */
static void TM_DMA_DirectModeErrorHandler(DMA_Stream_TypeDef* DMA_Stream) {
	/* Check if interrupt is for correct stream */
	if (DMA_Stream == TM_USART_DMA_GetStream(USART2)) {
		/* Do stuff here */
		//TM_USART_Puts(USART2, "Stream direct mode error occured\n");
	}
}

/* Called on FIFO error */
static void TM_DMA_FIFOErrorHandler(DMA_Stream_TypeDef* DMA_Stream) {
	/* Check if interrupt is for correct stream */
	if (DMA_Stream == TM_USART_DMA_GetStream(USART2)) {
		/* Do stuff here */
		//TM_USART_Puts(USART2, "FIFO error occured\n");
	}
}

/* Called when Custom TIMER1 reaches zero */
static void timeout_handler(void* UserParameters) 
{
	timeout_flag = true;
}

uint16_t usart_send_str(char *str)
{
    TM_USART_DMA_Send(USART2, (uint8_t *)str, strlen(str));
    while (TM_USART_DMA_Sending(USART2));
}

uint16_t usart_get_str(char *str, uint16_t len)
{
	return TM_USART_Gets(USART2, str, len);
}

bool app_gps_request_and_get_reply(char *str_request,
							 	   char *str_reply_expect, uint16_t len_reply_expect)
{
	char receive_data_temp[100];
	memset(receive_data_temp, 0, 100);
	timeout_flag = false;
	TM_DELAY_TimerStart(timeout_timer);

	usart_send_str(str_request);
	do
	{
		usart_get_str(receive_data_temp, sizeof(receive_data_temp));
	}
	while(memcmp(receive_data_temp, str_reply_expect, len_reply_expect) && !timeout_flag);
	TM_USART_ClearBuffer(USART2);
	TM_DELAY_TimerStop(timeout_timer);
	
	if(timeout_flag == false)
	{
		sprintf(&receive_data_temp[strlen(receive_data_temp)]," <=> %s\r\n", str_request);
		TM_USART_Puts(USART1, receive_data_temp);
	}
	else
	{
		memcpy(receive_data_temp, str_request, strlen(str_request));
		sprintf(&receive_data_temp[strlen(receive_data_temp)]," fail\r\n");
		TM_USART_Puts(USART1, receive_data_temp);
	}
	
	return !timeout_flag;
}

void app_gps_init(uint32_t baudrate_usart)
{
	/* Timer has reload value each 7s, enabled auto reload feature*/
	timeout_timer = TM_DELAY_TimerCreate(7000, 1, 0, timeout_handler, NULL);
	
	/* Init USART2 on pins TX = PA2, RX = PA3 */
	/* This pins are used on Nucleo boards for USB to UART via ST-Link */
	TM_USART_Init(USART2, TM_USART_PinsPack_1, baudrate_usart);
	TM_USART_Init(USART1, TM_USART_PinsPack_2, baudrate_usart);//pb6 pb7
	/* Init TX DMA for USART2 */
	/* Interrupts for USART2 DMA TX stream are also enabled */
	TM_USART_DMA_Init(USART2);
	
	/* Enable USART DMA interrupts */
	TM_USART_DMA_EnableInterrupts(USART2);
	
	TM_USART_Puts(USART1, "test uart 1\r\n");
	
	//while(!app_gps_request_and_get_reply("", "+CREG: 1\r\n", 10));
	
	while(!app_gps_request_and_get_reply("AT\r\n", "OK", 2));
	while(!app_gps_request_and_get_reply("AT+GPS=1\r\n", "OK", 2));
	while(!app_gps_request_and_get_reply("AT+GPSRD=1\r\n", "OK", 2));
	
	char temp_str[200];
	//AT+CIPSTART="TCP","184.106.153.149",80$0D$0A
	sprintf(temp_str, "AT+CIPSTART=\"TCP\",\"%s\",3000\r\n", IP_SERVER);
	while(!app_gps_request_and_get_reply(temp_str, "OK", 2));
}

void app_gps_get_value(char *str)
{
	char temp_char;
	bool b_gps_data_ready = false;
	
	uint16_t char_index = 0;
	uint16_t i =0;
	
	memset(gps_data, 0, 200);

	do
	{
		usart_get_str(gps_data, sizeof(gps_data));
	}
	while(memcmp(gps_data, "$GPRMC", 6));
	gb_gps_data_ready = true;
	TM_USART_ClearBuffer(USART2);
	memcpy(str, gps_data, sizeof(gps_data));
	TM_USART_Puts(USART1, gps_data);//check data of usart2 if receive data has "$GPRMC" string
	memset(gps_data, 0, 200);
}

void app_gprs_send_data(void)
{
	if(gb_gps_data_ready == true)
	{
		//while(!app_gps_request_and_get_reply("AT+CIPSEND\r\n", ">", 1));
		char temp_str[200];
		
		usart_send_str("AT+CIPSEND\r\n");
		
		Delayms(3000);		
		
		usart_send_str("POST /api_check_post HTTP/1.1\r\n");
		sprintf(temp_str, "Host: %s:3000\r\n", IP_SERVER);
		usart_send_str(temp_str);
		usart_send_str("Connection: keep-alive\r\n");
		usart_send_str("Content-Length: 12\r\n");
		usart_send_str("Accept: */*\r\n");
		sprintf(temp_str, "Origin: http://%s:3000\r\n", IP_SERVER);
		usart_send_str(temp_str);
		usart_send_str("X-Requested-With: XMLHttpRequest\r\n");
		usart_send_str("User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.102 Safari/537.36\r\n");
		usart_send_str("Content-Type: application/x-www-form-urlencoded; charset=UTF-8\r\n");
		sprintf(temp_str, "Referer: http://%s:3000/\r\n", IP_SERVER);
		usart_send_str(temp_str);
		usart_send_str("Accept-Encoding: gzip, deflate\r\n");
		usart_send_str("Accept-Language: vi-VN,vi;q=0.9,fr-FR;q=0.8,fr;q=0.7,en-US;q=0.6,en;q=0.5\r\n");
		usart_send_str("Cookie: user_sid=s%3AfH8z6KfjyKk_hyG44jeKUpuEL4YFuXvr.dPaiKGI%2FiGL6ZS78yx%2F%2B9OhfuoELaxChNZ%2FKjsVSS2o; io=juD-GQgrjeUvVPJdAAAB\r\n\r\n");
		sprintf(&gps_data[strlen(gps_data)], "%c", 0x1A);
		app_gps_request_and_get_reply(gps_data,"ok",2);
		memset(gps_data, 0, sizeof(gps_data));
		gb_gps_data_ready = false;

  }
}
