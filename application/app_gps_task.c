#include "app_gps_task.h"

/* Variables used */

static TM_DELAY_Timer_t* timeout_timer;

// init AT command here
static char OPEN_GPS_AT[10] = "AT+GPS=1\r\n";
static char CLOSE_GPS_AT[8] = "AT+GPS=0";
static char OPEN_AGPS_AT[9] = "AT+AGPS=1";
static char CLOSE_AGPS_AT[9] = "AT+AGPS=0";
static char receive_data[100] = "receiver\n";
static bool	timeout_flag = false;
/* Called when transfer is completed for specific stream */
static void TM_DMA_TransferCompleteHandler(DMA_Stream_TypeDef* DMA_Stream) {
	/* Check if interrupt is for correct stream */
	if (DMA_Stream == TM_USART_DMA_GetStream(USART2)) {
		TM_USART_Puts(USART2, "Stream has finished with transfer\n");
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

uint16_t usart_send_str(char *str, uint16_t len)
{
    TM_USART_DMA_Send(USART2, (uint8_t *)str, len);
    while (TM_USART_DMA_Sending(USART2));
}

uint16_t usart_get_str(char *str, uint16_t len)
{
    return TM_USART_Gets(USART2, str, len)
}

bool app_gps_request_and_get_reply(char *str_request, uint16_t len_request,
							 	   char *str_reply, uint16_t len_reply)
{
	char receive_data_temp[100] = "\r\n";
	timeout_flag = false;
	TM_DELAY_TimerStart(timeout_timer);
	do
    {
		usart_send_str(str_request, len_request);
		usart_get_str(receive_data_temp, len_reply);
	}
	while(!memcmp(receive_data_temp, str_reply, len_reply) && !timeout_flag);
	TM_DELAY_TimerStop(timeout_timer);
	return !timeout_flag;
}

void app_gps_init(void)
{
	/* Timer has reload value each 5s, enabled auto reload feature*/
	timeout_timer = TM_DELAY_TimerCreate(5000, 1, 0, timeout_handler, NULL);
	
	/* Init USART2 on pins TX = PA2, RX = PA3 */
	/* This pins are used on Nucleo boards for USB to UART via ST-Link */
	TM_USART_Init(USART2, TM_USART_PinsPack_1, 115200);
	
	/* Say string without DMA */
	TM_USART_Puts(USART2, "Hello via USART2 without DMA\n");
	
	/* Init TX DMA for USART2 */
	/* Interrupts for USART2 DMA TX stream are also enabled */
	TM_USART_DMA_Init(USART2);
	
	/* Enable USART DMA interrupts */
	TM_USART_DMA_EnableInterrupts(USART2);

	while (1)
	{
		/* If any string arrived over USART */
		/* Expecting "\n" at the end of string from USART terminal or any other source */
		if (TM_USART_Gets(USART2, USART_Buffer, sizeof(USART_Buffer))) 
		{
			/* Send it back over DMA */
			TM_USART_DMA_Send(USART2, (uint8_t *)USART_Buffer, strlen(USART_Buffer));

			/* Wait till DMA works */
			/* You can do other stuff here instead of waiting for DMA to end */
			while (TM_USART_DMA_Sending(USART2));
		}
	}
}

void app_gps_get_value(char *str, uint16_t len)
{
    
}

