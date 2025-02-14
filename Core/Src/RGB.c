#include "RGB.h"
#include "stm32g4xx_it.h"
#include "stm32g4xx_hal.h"
#include "stdlib.h"
#include "math.h"

extern TIM_HandleTypeDef htim17;
extern TIM_HandleTypeDef htim16;
extern uint8_t ShortPress;
extern uint8_t LongPress;
extern uint8_t HoldPress;
extern uint8_t HoldCount;

static uint32_t press_time = 0;
uint32_t release_time = 0;
uint32_t elapsed_time = 0;
static uint32_t overflow_count = 0;


void HandleButtonPress (GPIO_TypeDef *GPIO, uint16_t GPIO_Pin)
{
	if (HAL_GPIO_ReadPin (GPIO, GPIO_Pin) == GPIO_PIN_SET)
	{
		if (press_time == 0) {
			press_time = __HAL_TIM_GET_COUNTER (&htim16);
			overflow_count = 0;
			HoldPress = 0;
			HAL_TIM_Base_Start_IT (&htim16);
		}
	}

	elapsed_time = (overflow_count * 500) + (__HAL_TIM_GET_COUNTER (&htim16) - press_time);
	if (elapsed_time >= 500)
	{
		if (!HoldPress) {
			HoldPress = 1;
			HandleHoldPress();
		}

		if (elapsed_time % 500 == 0) {
			HandleHoldPress();
		}
	}

	if (HAL_GPIO_ReadPin (GPIO, GPIO_Pin) == GPIO_PIN_RESET)
	{
		release_time = __HAL_TIM_GET_COUNTER (&htim16);
		HAL_TIM_Base_Stop_IT (&htim16);

		elapsed_time = (overflow_count * 500) + (release_time - press_time);

		__HAL_TIM_SET_COUNTER (&htim16, 0);
		press_time = 0;
		release_time = 0;
		overflow_count = 0;
		HoldPress = 0;
	}

	if (elapsed_time < 20)
	{} else if (elapsed_time >= 20 && elapsed_time < 200) {
		ShortPress = 1;
	} else  if (elapsed_time >= 200 && elapsed_time < 500) {
		LongPress = 1;
	}
}

void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim)
{
	if (htim -> Instance == TIM16)
	{
		overflow_count++;
	}
}

void HandleHoldPress (void)
{
	HoldCount++;
}
