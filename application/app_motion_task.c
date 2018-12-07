#include "app_motion_task.h"

static uint8_t TX_Buffer[200], RX_Buffer[200];

void app_motion_init(void)
{
	//init chip select
	TM_GPIO_Init(GPIOE, GPIO_Pin_3, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High);
	TM_GPIO_SetPinLow(GPIOE, GPIO_Pin_3);//PE3
	
	/* Init SPI
	* SCK = PA5, MOSI = PA7, MISO = PA6	
	* @note: it's same pin pwm motor2, so uinit spi when init lis3dsh
	*/
	TM_SPI_Init(SPI1, TM_SPI_PinsPack_1);
	
	/* Set fake SPI TX buffer */
	memset(TX_Buffer, 0x00, sizeof(TX_Buffer));
	
	/* Send data over SPI1 with DMA */
	/* Exchange data with SPI slave using SPI DMA */
	/* Exchange 5 bytes of data */
	TM_SPI_DMA_Transmit(SPI1, TX_Buffer, RX_Buffer, sizeof(TX_Buffer));
	
	/* Wait till SPI DMA do it's job */
	/* You can do other stuff instead */
	while (TM_SPI_DMA_Working(SPI1));
	
	TM_SPI_SendMulti(SPI1, TX_Buffer, RX_Buffer, sizeof(TX_Buffer));
	
}