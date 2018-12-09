#include "app_mpu6050_task.h"

static float accel_scale_value = 0.0;   // Accelerometer corrector from raw data to "g"
static float gyro_scale_value = 0.0;     // Gyroscope corrector from raw data to "degrees/s"

MPU6050_Result_t app_mpu_6050_init(uint8_t accel_sensitivity,
								   uint8_t gyro_sensitivity)
{
	uint8_t data_temp = 0xFF;
    /* Initialize I2C3 */
	// pin PA8: SCL, PC9: SDA
    TM_I2C_Init(I2C3, TM_I2C_PinsPack_1, MPU6050_I2C_CLOCK);

    	/* Check if device is connected */
	if (!TM_I2C_IsDeviceConnected(I2C3, MPU6050_I2C_ADDR)) 
  	{
		/* Return error */
		return MPU6050_DeviceNotConnected;
	}

		/* Check who I am */
	if (TM_I2C_Read(I2C3, MPU6050_I2C_ADDR, MPU6050_WHO_AM_I) != MPU6050_I_AM) 
	{
		/* Return error */
		return MPU6050_DeviceInvalid;
	}

	/* Wakeup MPU6050 */
	TM_I2C_Write(I2C3, MPU6050_I2C_ADDR, MPU6050_PWR_MGMT_1, 0x00);

	/* set sample time : 8kHz/(79+1)=100Hz */
	TM_I2C_Write(I2C3, MPU6050_I2C_ADDR, MPU6050_SMPLRT_DIV, 79);

	/* Config accelerometer */
	data_temp = TM_I2C_Read(I2C3, MPU6050_I2C_ADDR, MPU6050_ACCEL_CONFIG);
	data_temp = (data_temp & 0xE7) | accel_sensitivity << 3;
	TM_I2C_Write(I2C3, MPU6050_I2C_ADDR, MPU6050_ACCEL_CONFIG, data_temp);
	
	/* Config gyroscope */
	data_temp = TM_I2C_Read(I2C3, MPU6050_I2C_ADDR, MPU6050_GYRO_CONFIG);
	data_temp = (data_temp & 0xE7) | gyro_sensitivity << 3;
	TM_I2C_Write(I2C3, MPU6050_I2C_ADDR, MPU6050_GYRO_CONFIG, data_temp);

	switch (accel_sensitivity)
	{
		case MPU6050_Accelerometer_2G:
			accel_scale_value = (float)1 / MPU6050_ACCE_SENS_2; 
			break;
		case MPU6050_Accelerometer_4G:
			accel_scale_value = (float)1 / MPU6050_ACCE_SENS_4; 
			break;
		case MPU6050_Accelerometer_8G:
			accel_scale_value = (float)1 / MPU6050_ACCE_SENS_8; 
			break;
		case MPU6050_Accelerometer_16G:
			accel_scale_value = (float)1 / MPU6050_ACCE_SENS_16; 
			break;
		default:
			break;
	}
	
	switch (gyro_sensitivity) 
	{
		case MPU6050_Gyroscope_250s:
			gyro_scale_value = (float)1 / MPU6050_GYRO_SENS_250;
			break;
		case MPU6050_Gyroscope_500s:
			gyro_scale_value = (float)1 / MPU6050_GYRO_SENS_500; 
			break;
		case MPU6050_Gyroscope_1000s:
			gyro_scale_value = (float)1 / MPU6050_GYRO_SENS_1000; 
			break;
		case MPU6050_Gyroscope_2000s:
			gyro_scale_value = (float)1 / MPU6050_GYRO_SENS_2000; 
			break;
		default:
			break;
	}
	
	/* Return OK */
	return MPU6050_Ok;
}

MPU6050_Result_t app_mpu6050_ReadAll(MPU6050_data_t* DataStruct) 
{
	uint8_t data[14];
	int16_t temp;
	
	/* Read full raw data, 14bytes */
	TM_I2C_ReadMulti(I2C3, MPU6050_I2C_ADDR, MPU6050_ACCEL_XOUT_H, data, 14);
	
	/* Format accelerometer data */
	DataStruct->Accelerometer_X = (float)((int16_t)(data[0] << 8 | data[1]))*accel_scale_value;
	DataStruct->Accelerometer_Y = (float)((int16_t)(data[2] << 8 | data[3]))*accel_scale_value;
	DataStruct->Accelerometer_Z = (float)((int16_t)(data[4] << 8 | data[5]))*accel_scale_value;

	/* Format temperature */
	temp = (data[6] << 8 | data[7]);
	DataStruct->Temperature = (float)((float)((int16_t)temp) / (float)340.0 + (float)36.53);
	
	/* Format gyroscope data */
	DataStruct->Gyroscope_X = (float)((int16_t)(data[8] << 8 | data[9]))*gyro_scale_value;
	DataStruct->Gyroscope_Y = (float)((int16_t)(data[10] << 8 | data[11]))*gyro_scale_value;
	DataStruct->Gyroscope_Z = (float)((int16_t)(data[12] << 8 | data[13]))*gyro_scale_value;

	/* Return OK */
	return MPU6050_Ok;
}
