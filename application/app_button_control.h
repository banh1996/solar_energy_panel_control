#ifndef __BUTTON_CONTROL
#define __BUTTON_CONTROL

#include <stdio.h>
#include <stdbool.h>
#include "tm_stm32f4_disco.h"

/* brief: init button board
*/
void app_button_init(void);

/* brief: check status button board
*/
bool app_button_read_state(void);

#endif