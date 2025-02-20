#include "RGB.h"
#include "stm32g4xx_it.h"
#include "stm32g4xx_hal.h"
#include "stdlib.h"
#include "math.h"

extern TIM_HandleTypeDef htim17;
extern TIM_HandleTypeDef htim16;

uint32_t release_time = 0;
static uint8_t overflow_count = 0;
ButtonState CurrentState = STATIC;
uint8_t release_flag = 0;

void HandleButtonPress (GPIO_TypeDef *GPIO, uint16_t GPIO_Pin)
{
	if (HAL_GPIO_ReadPin (GPIO, GPIO_Pin) == GPIO_PIN_SET)
	{
		__HAL_TIM_SET_COUNTER(&htim16, 0);
		overflow_count = 0;
		release_flag = 0;
		HAL_TIM_Base_Start_IT (&htim16);
	}

	if (HAL_GPIO_ReadPin (GPIO, GPIO_Pin) == GPIO_PIN_RESET)
	{
		release_time = __HAL_TIM_GET_COUNTER (&htim16) + (overflow_count * 100);
		release_flag = 1;
	}
}

void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim)
{
	if (htim -> Instance == TIM16)
	{
		overflow_count++;

		if (release_flag == 1)
		{
			if (overflow_count >= 0 && overflow_count < 2)
			{
				CurrentState = SHORT;
			} else if (overflow_count > 2 && overflow_count < 5) {
				CurrentState = LONG;
			} else if (overflow_count >= 5)
			{
				CurrentState = HOLDING;
			}
		} else {
			CurrentState = STATIC;
		}
	}
}
