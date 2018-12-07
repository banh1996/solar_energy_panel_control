#ifndef __MOTION_TASK
#define __MOTION_TASK

#include <stdio.h>
#include <stdbool.h>
#include "string.h"

#include "tm_stm32f4_spi.h"
#include "tm_stm32f4_gpio.h"

/* SPI on STM32F4-Discovery board */
#ifndef LIS302DL_LIS3DSH_SPI
#define LIS302DL_LIS3DSH_SPI				SPI1
#define LIS302DL_LIS3DSH_SPI_PINSPACK		TM_SPI_PinsPack_1
#endif

/* CS pin on STM32F4-Discovery board */
#ifndef LIS302DL_LIS3DSH_CS_PIN
#define LIS302DL_LIS3DSH_CS_RCC				RCC_AHB1Periph_GPIOE
#define LIS302DL_LIS3DSH_CS_PORT			GPIOE
#define LIS302DL_LIS3DSH_CS_PIN				GPIO_Pin_3
#endif

/* CS pin settings */
#define LIS302DL_LIS3DSH_CS_LOW				LIS302DL_LIS3DSH_CS_PORT->BSRRH = LIS302DL_LIS3DSH_CS_PIN
#define LIS302DL_LIS3DSH_CS_HIGH			LIS302DL_LIS3DSH_CS_PORT->BSRRL = LIS302DL_LIS3DSH_CS_PIN

/* Who I am values */
#define LIS302DL_ID							0x3B
#define LIS3DSH_ID							0x3F

/* Common registers */
#define LIS302DL_LIS3DSH_REG_WHO_I_AM		0x0F

/* ----------------------------------------- */
/* LIS3DSH registers */
/* ----------------------------------------- */
#define LIS3DSH_WHO_AM_I_ADDR				0x0F
#define LIS3DSH_CTRL_REG4_ADDR				0x20
#define LIS3DSH_CTRL_REG1_ADDR				0x21
#define LIS3DSH_CTRL_REG2_ADDR				0x22
#define LIS3DSH_CTRL_REG3_ADDR				0x23
#define LIS3DSH_CTRL_REG5_ADDR				0x24
#define LIS3DSH_CTRL_REG6_ADDR				0x25
#define LIS3DSH_OUT_X_L_ADDR				0x28
#define LIS3DSH_OUT_X_H_ADDR				0x29
#define LIS3DSH_OUT_Y_L_ADDR				0x2A
#define LIS3DSH_OUT_Y_H_ADDR				0x2B
#define LIS3DSH_OUT_Z_L_ADDR				0x2C
#define LIS3DSH_OUT_Z_H_ADDR				0x2D

#define LIS3DSH_SENSITIVITY_0_06G            0.06  /* 0.06 mg/digit*/
#define LIS3DSH_SENSITIVITY_0_12G            0.12  /* 0.12 mg/digit*/
#define LIS3DSH_SENSITIVITY_0_18G            0.18  /* 0.18 mg/digit*/
#define LIS3DSH_SENSITIVITY_0_24G            0.24  /* 0.24 mg/digit*/
#define LIS3DSH_SENSITIVITY_0_73G            0.73  /* 0.73 mg/digit*/

#define LIS3DSH_DATARATE_100				((uint8_t)0x60)

#define LIS3DSH_FULLSCALE_2					((uint8_t)0x00)  /* 2 g  */
#define LIS3DSH_FULLSCALE_4					((uint8_t)0x08)  /* 4 g  */
#define LIS3DSH_FULLSCALE_6					((uint8_t)0x10)  /* 6 g  */
#define LIS3DSH_FULLSCALE_8					((uint8_t)0x18)  /* 8 g  */
#define LIS3DSH_FULLSCALE_16				((uint8_t)0x20)  /* 16 g */
#define LIS3DSH__FULLSCALE_SELECTION		((uint8_t)0x38)

#define LIS3DSH_FILTER_BW_800				((uint8_t)0x00)  /* 800 Hz */
#define LIS3DSH_FILTER_BW_400				((uint8_t)0x40)//((uint8_t)0x08) /* 400 Hz  */
#define LIS3DSH_FILTER_BW_200				((uint8_t)0x80)//((uint8_t)0x10)  /* 200 Hz */
#define LIS3DSH_FILTER_BW_50				((uint8_t)(0x80 | 0x40))//((uint8_t)0x18)  /* 50 Hz  */
#define LIS3DSH_SELFTEST_NORMAL				((uint8_t)0x00)
#define LIS3DSH_XYZ_ENABLE					((uint8_t)0x07)
#define LIS3DSH_SERIALINTERFACE_4WIRE		((uint8_t)0x00)
#define LIS3DSH_SM_ENABLE					((uint8_t)0x01)
#define LIS3DSH_SM_DISABLE					((uint8_t)0x00)

typedef enum {
	/* LIS3DSH */
	TM_LIS3DSH_Filter_800Hz,
	TM_LIS3DSH_Filter_400Hz,
	TM_LIS3DSH_Filter_200Hz,
	TM_LIS3DSH_Filter_50Hz,
	/* LIS302DL */
	TM_LIS302DL_Filter_2Hz,
	TM_LIS302DL_Filter_1Hz,
	TM_LIS302DL_Filter_500mHz,
	TM_LIS302DL_Filter_250mHz
} TM_LIS302DL_LIS3DSH_Filter_t;

void app_motion_init(void);

#endif