#include "app_gps_task.h"

/* Variables used */
// init AT command here
static char OPEN_GPS_AT[10] = "AT+GPS=1\r\n";
static char CLOSE_GPS_AT[8] = "AT+GPS=0";
static char OPEN_AGPS_AT[9] = "AT+AGPS=1";
static char CLOSE_AGPS_AT[9] = "AT+AGPS=0";

void usart_send_str(char *str, uint16_t len)
{
    TM_USART_DMA_Send(USART1, (uint8_t *)str, len);
    while (TM_USART_DMA_Sending(USART1));
}

uint16_t usart_get_str(char *str, uint16_t len)
{
    uint16_t err = TM_USART_Gets(USART1, str, len);
		TM_USART_BufferEmpty(USART1);
		return err;
}

void app_gps_init(void)
{

  /* Initialize USART1 for debug */
	/* TX = PB6 , RD = PB7*/
	TM_USART_Init(USART1, TM_USART_PinsPack_2, 9600);
	TM_USART_DMA_Init(USART1);

	//connect D_SEL with VDD to select interface USART

	//Open GPS
	usart_send_str(OPEN_GPS_AT, 10);
	
	/* Reset counter */
	TM_DELAY_SetTime(0);
	
	return;
}

void app_gps_get_value(char *str, uint16_t len)
{
    
}



