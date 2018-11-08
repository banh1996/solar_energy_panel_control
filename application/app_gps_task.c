#include "app_gps_task.h"

typedef struct {
	uint8_t *Buffer;
	uint16_t Size;
	uint16_t Num;
	uint16_t In;
	uint16_t Out;
	uint8_t Initialized;
	uint8_t StringDelimiter;
} TM_USART_t;

/* Variables used */
static TM_DELAY_Timer_t* timeout_timer;

// init AT command here
static char OPEN_GPS_AT[8] = "AT+GPS=1";
static char CLOSE_GPS_AT[8] = "AT+GPS=0";
static char OPEN_AGPS_AT[9] = "AT+AGPS=1";
static char CLOSE_AGPS_AT[9] = "AT+AGPS=0";
static char receive_data[100] = "receiver\n";
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

uint16_t usart_send_str(char *str, uint16_t len)
{
    TM_USART_DMA_Send(USART2, (uint8_t *)str, len);
    while (TM_USART_DMA_Sending(USART2));
}

uint16_t usart_get_str(char *str, uint16_t len)
{
	return TM_USART_Gets(USART2, str, len);
}

bool app_gps_request_and_get_reply(char *str_request, uint16_t len_request,
							 	   char *str_reply_expect, uint16_t len_reply_expect)
{
	char receive_data_temp[100];
	memset(receive_data_temp, 0, 100);
	timeout_flag = false;
	TM_DELAY_TimerStart(timeout_timer);

	usart_send_str(str_request, len_request);
	do
	{
		usart_get_str(receive_data_temp, sizeof(receive_data_temp));
	}
	while(memcmp(receive_data_temp, str_reply_expect, len_reply_expect) && !timeout_flag);
	TM_USART_ClearBuffer(USART2);
	TM_DELAY_TimerStop(timeout_timer);

	return !timeout_flag;
}

void app_gps_init(uint32_t baudrate_usart)
{
	/* Timer has reload value each 5s, enabled auto reload feature*/
	timeout_timer = TM_DELAY_TimerCreate(3000, 1, 0, timeout_handler, NULL);
	
	/* Init USART2 on pins TX = PA2, RX = PA3 */
	/* This pins are used on Nucleo boards for USB to UART via ST-Link */
	TM_USART_Init(USART2, TM_USART_PinsPack_1, baudrate_usart);
	
	/* Init TX DMA for USART2 */
	/* Interrupts for USART2 DMA TX stream are also enabled */
	TM_USART_DMA_Init(USART2);
	
	/* Enable USART DMA interrupts */
	TM_USART_DMA_EnableInterrupts(USART2);

}

uint16_t app_gps_get_value(char *str)
{
	char receive_data_temp[300];
	char receive_data_temp2[300];
	char temp_char;
	bool b_gps_data_ready = false;
	
	uint16_t char_index = 0;
	uint16_t i =0;
	
	memset(receive_data_temp, 0, 300);
	memset(receive_data_temp2, 0, 300);
	while (1) 
	{	
		do
		{
			usart_get_str(receive_data_temp, sizeof(receive_data_temp));
		}
		while(memcmp(receive_data_temp, "GPRMC", 5));
		usart_send_str(receive_data_temp, sizeof(receive_data_temp));
		memset(receive_data_temp, 0, 300);
	}
	
//  do
//	{
//		usart_get_str(receive_data_temp, sizeof(receive_data_temp));
//	}
//	while(memcmp(receive_data_temp, "GPRMC", 5));
//	
//	memset(receive_data_temp, 0, sizeof(receive_data_temp));
//	
//	do
//	{
//		char_index = usart_get_str(receive_data_temp, sizeof(receive_data_temp));
//	}
//	while(receive_data_temp[char_index-1] != '\n');
//	
//	TM_USART_ClearBuffer(USART2);
//	
//	usart_send_str(receive_data_temp, sizeof(receive_data_temp));
//	
//	memcpy(str, receive_data_temp, char_index);
}

//uint16_t app_gps_get(char* buffer, uint16_t bufsize) 
//{
//	uint16_t i = 0;
//	
//	/* Get USART structure */
//	TM_USART_t* u = TM_USART_INT_GetUsart(USART2);
//	
//	/* Check for any data on USART */
//	if (
//		u->Num == 0 ||                                             /*!< Buffer empty */
//		(
//			!TM_USART_FindCharacter(USART2, u->StringDelimiter) && /*!< String delimiter not in buffer */
//			u->Num != u->Size                                      /*!< Buffer is not full */
//		)
//	) 
//	{
//		/* Return 0 */
//		return 0;
//	}
//	
//	/* If available buffer size is more than 0 characters */
//	while (i < (bufsize - 1)) 
//	{
//		/* We have available data */
//		buffer[i] = (char) TM_USART_Getc(USART2);
//		
//		/* Check for end of string */
//		if ((uint8_t) buffer[i] == (uint8_t) u->StringDelimiter) {
//			/* Done */
//			break;
//		}
//		
//		/* Increase */
//		i++;
//	}
//	
//	/* Add zero to the end of string */
//	buffer[++i] = 0;               

//	/* Return number of characters in buffer */
//	return i;
//}