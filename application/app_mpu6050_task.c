#include "app_mpu6050_task.h"


void app_mpu_6050_init(void)
{
    /* Initialize I2C */
    TM_I2C_Init(MPU6050_I2C, MPU6050_I2C_PINSPACK, MPU6050_I2C_CLOCK);

    	/* Check if device is connected */
	if (!TM_I2C_IsDeviceConnected(MPU6050_I2C, MPU6050_I2C_ADDR)) 
    {
		/* Return error */
		return TM_MPU6050_Result_DeviceNotConnected;
	}
    
}