#ifndef MPU6xx__
#define MPU6xx__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "tm_stm32f4_i2c.h"

/* Default I2C clock */
#ifndef MPU6050_I2C_CLOCK
#define MPU6050_I2C_CLOCK			400000
#endif

/* Default I2C address */
#define MPU6050_I2C_ADDR			0xD0

/* Who I am register value */
#define MPU6050_I_AM				0x68

/* MPU6050 registers */
#define MPU6050_AUX_VDDIO			0x01
#define MPU6050_SMPLRT_DIV			0x19
#define MPU6050_CONFIG				0x1A
#define MPU6050_GYRO_CONFIG			0x1B
#define MPU6050_ACCEL_CONFIG		0x1C
#define MPU6050_MOTION_THRESH		0x1F
#define MPU6050_INT_PIN_CFG			0x37
#define MPU6050_INT_ENABLE			0x38
#define MPU6050_INT_STATUS			0x3A
#define MPU6050_ACCEL_XOUT_H		0x3B
#define MPU6050_ACCEL_XOUT_L		0x3C
#define MPU6050_ACCEL_YOUT_H		0x3D
#define MPU6050_ACCEL_YOUT_L		0x3E
#define MPU6050_ACCEL_ZOUT_H		0x3F
#define MPU6050_ACCEL_ZOUT_L		0x40
#define MPU6050_TEMP_OUT_H			0x41
#define MPU6050_TEMP_OUT_L			0x42
#define MPU6050_GYRO_XOUT_H			0x43
#define MPU6050_GYRO_XOUT_L			0x44
#define MPU6050_GYRO_YOUT_H			0x45
#define MPU6050_GYRO_YOUT_L			0x46
#define MPU6050_GYRO_ZOUT_H			0x47
#define MPU6050_GYRO_ZOUT_L			0x48
#define MPU6050_MOT_DETECT_STATUS	0x61
#define MPU6050_SIGNAL_PATH_RESET	0x68
#define MPU6050_MOT_DETECT_CTRL		0x69
#define MPU6050_USER_CTRL			0x6A
#define MPU6050_PWR_MGMT_1			0x6B
#define MPU6050_PWR_MGMT_2			0x6C
#define MPU6050_FIFO_COUNTH			0x72
#define MPU6050_FIFO_COUNTL			0x73
#define MPU6050_FIFO_R_W			0x74
#define MPU6050_WHO_AM_I			0x75

/* Gyro sensitivities in °/s */
#define MPU6050_GYRO_SENS_250		((float) 131)
#define MPU6050_GYRO_SENS_500		((float) 65.5)
#define MPU6050_GYRO_SENS_1000		((float) 32.8)
#define MPU6050_GYRO_SENS_2000		((float) 16.4)

/* Acce sensitivities in g */
#define MPU6050_ACCE_SENS_2			((float) 16384)
#define MPU6050_ACCE_SENS_4			((float) 8192)
#define MPU6050_ACCE_SENS_8			((float) 4096)
#define MPU6050_ACCE_SENS_16		((float) 2048)

#define MPU6050_Accelerometer_2G  	0x00 /*!< Range is +- 2G */
#define	MPU6050_Accelerometer_4G  	0x01 /*!< Range is +- 4G */
#define	MPU6050_Accelerometer_8G  	0x02 /*!< Range is +- 8G */
#define	MPU6050_Accelerometer_16G  	0x03 /*!< Range is +- 16G */

#define	MPU6050_Gyroscope_250s  		0x00  /*!< Range is +- 250 degrees/s */
#define	MPU6050_Gyroscope_500s  		0x01  /*!< Range is +- 500 degrees/s */
#define	MPU6050_Gyroscope_1000s  		0x02 /*!< Range is +- 1000 degrees/s */
#define	MPU6050_Gyroscope_2000s  		0x03  /*!< Range is +- 2000 degrees/s */

/**
 * @brief  MPU6050 result enumeration	
 */
typedef enum 
{
	MPU6050_Ok = 0x00,          /*!< Everything OK */
	MPU6050_DeviceNotConnected, /*!< There is no device with valid slave address */
	MPU6050_DeviceInvalid       /*!< Connected device with address is not MPU6050 */
} MPU6050_Result_t;

typedef struct 
{
	float Accelerometer_X; /*!< Accelerometer value X axis */
	float Accelerometer_Y; /*!< Accelerometer value Y axis */
	float Accelerometer_Z; /*!< Accelerometer value Z axis */
	float Gyroscope_X;     /*!< Gyroscope value X axis */
	float Gyroscope_Y;     /*!< Gyroscope value Y axis */
	float Gyroscope_Z;     /*!< Gyroscope value Z axis */
	float Temperature;       /*!< Temperature in degrees */
} MPU6050_data_t;

MPU6050_Result_t app_mpu_6050_init(uint8_t accel_sensitivity,
								   uint8_t gyro_sensitivity);

MPU6050_Result_t app_mpu6050_ReadAll(MPU6050_data_t* DataStruct);

#endif