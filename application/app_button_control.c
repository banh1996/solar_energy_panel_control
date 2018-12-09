#include "app_button_control.h"

void app_button_init(void)
{
	TM_DISCO_ButtonInit();
}

bool app_button_read_state(void)
{
	return TM_DISCO_ButtonOnPressed();
}
