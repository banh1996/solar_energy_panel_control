#ifndef __MOTOR_CONTROL
#define __MOTOR_CONTROL

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "tm_stm32f4_pwm.h"
#include "tm_stm32f4_delay.h"

#define MOTOR1         1
#define MOTOR2         2
#define SPEED_PERCENT  12
#define STOP_THRESHOLD 180

/* brief: function for init PWM servo
*/
void app_motor_init(uint16_t frequency);

/* brief: function for start motor
 * params: motor: MOTOR1 - choose motor 1  
 *                MOTOR2 - choose motor 2
 *         speed: set time on value speed/frequency
 *         isforward: true - motor twirls forward
 *                    false - motor twirls reverse  
*/
void app_motor_start(uint8_t motor, uint16_t percent, bool isforward);

/* brief: function for stop motor
 * params: motor: MOTOR1 - choose motor 1  
 *                MOTOR2 - choose motor 2
*/
void app_motor_stop(uint8_t motor);

void app_motor_control_servo(uint16_t adc0,
														 uint16_t adc1,
														 uint16_t adc2,
														 uint16_t adc3);
#endif