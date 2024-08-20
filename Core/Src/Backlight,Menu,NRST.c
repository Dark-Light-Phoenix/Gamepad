#include "Backlight,Menu,NRST.h"
#include "stm32g4xx_it.h"

void Backlight (float *Backlight)
{
	HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_5);
	HAL_Delay (500);
}
