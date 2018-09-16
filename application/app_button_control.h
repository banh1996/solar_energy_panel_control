#ifndef __BUTTON_CONTROL
#define __BUTTON_CONTROL

#include <stdio.h>
#include <stdbool.h>
#include "tm_stm32f4_disco.h"

void app_button_init(void);
bool app_button_read_state(void);

#endif