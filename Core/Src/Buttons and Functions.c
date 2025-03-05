#include "Buttons and Functions.h"
#include "stm32g4xx_it.h"
#include "stm32g4xx_hal.h"
#include "stdlib.h"
#include "math.h"
#include "stdbool.h"

extern TIM_HandleTypeDef htim17;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim7;

ButtonState CurrentState = STATIC;

extern volatile uint16_t last_gpioa_state;
extern volatile uint16_t last_gpiob_state;
extern volatile uint16_t previous_gpioa_state;
extern volatile uint16_t previous_gpiob_state;

volatile bool press = false;
volatile uint32_t overflow_count = 0;

uint8_t changed_statea_3 = 0;
uint8_t changed_statea_5 = 0;
uint8_t changed_statea_8 = 0;
uint8_t changed_statea_9 = 0;
uint8_t changed_statea_10 = 0;
uint8_t changed_statea_13 = 0;
uint8_t changed_statea_14 = 0;
uint8_t changed_statea_15 = 0;

uint8_t changed_stateb_0 = 0;
uint8_t changed_stateb_3 = 0;
uint8_t changed_stateb_4 = 0;
uint8_t changed_stateb_5 = 0;
uint8_t changed_stateb_6 = 0;
uint8_t changed_stateb_7 = 0;

uint8_t changed_stateg_10 = 0;

static uint16_t check_status = 0;

void HandleButtonPress (GPIO_TypeDef *GPIO, uint16_t GPIO_Pin)
{
	if (HAL_GPIO_ReadPin (GPIO, GPIO_Pin) == GPIO_PIN_RESET)
	{
		__HAL_TIM_SET_COUNTER(&htim16, 0);
		overflow_count = 0;
		check_status = 1;
	}

	if (HAL_GPIO_ReadPin (GPIO, GPIO_Pin) == GPIO_PIN_SET)
	{
		check_status = 0;
	}

//	if (CurrentTime = SHORT)
//	{
//
//	} else if (CurrentTime = LONG) {
//
//	}
}



void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim)
{
	if (htim -> Instance == TIM16)
	{
		overflow_count++;

		if (check_status)
		{
			if (__HAL_TIM_GET_COUNTER (&htim16) + (overflow_count * 100) < 40) {
				CurrentState = STATIC;
			} if (__HAL_TIM_GET_COUNTER (&htim16) + (overflow_count * 100) >= 40 && __HAL_TIM_GET_COUNTER (&htim16) + (overflow_count * 100) < 500) {
				CurrentState = SHORT;
			} if (__HAL_TIM_GET_COUNTER (&htim16) + (overflow_count * 100) >= 500 && __HAL_TIM_GET_COUNTER (&htim16) + (overflow_count * 100) < 5000) {
				CurrentState = LONG;
		}
	}

	if (htim -> Instance == TIM7)
	{
		if (HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_3) == GPIO_PIN_SET) {
			changed_statea_3 = 1;
		} else if (HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_3) == GPIO_PIN_RESET) {
			changed_statea_3 = 0;
		}
	}
	}
}

void ButtonStatus (void)
{
	last_gpioa_state = GPIOA->IDR & GPIOA_MASK;
	last_gpiob_state = GPIOB->IDR & GPIOB_MASK;

	uint16_t changed_pins_a = last_gpioa_state ^ previous_gpioa_state;
	uint16_t relevant_changes_a = changed_pins_a & GPIOA_MASK;
	uint16_t changed_pins_b = last_gpiob_state ^ previous_gpiob_state;
	uint16_t relevant_changes_b = changed_pins_b & GPIOB_MASK;

	if (relevant_changes_a & GPIO_PIN_5) // BACKLIGHT
	{
		if (last_gpioa_state & GPIO_PIN_5) {
			changed_statea_5 = 1;
			HandleButtonPress(GPIOA, GPIO_PIN_5);
		} else {
			changed_statea_5 = 0;
			HandleButtonPress(GPIOA, GPIO_PIN_5);
		}
	}

//---------------------------------------------------------------CHREST/TRIANGLE/CIRCLE/SQUARE START

	if (relevant_changes_a & GPIO_PIN_8) // CHREST
	{
		if (last_gpioa_state & GPIO_PIN_8) {
			changed_statea_8 = 1;
			HandleButtonPress(GPIOA, GPIO_PIN_8);
		} else {
			changed_statea_8 = 0;
			HandleButtonPress(GPIOA, GPIO_PIN_8);
		}
	}

	if (relevant_changes_a & GPIO_PIN_9) // TRIAGLE
	{
		if (last_gpioa_state & GPIO_PIN_9) {
			changed_statea_9 = 1;
			HandleButtonPress(GPIOA, GPIO_PIN_9);
		} else {
			changed_statea_9 = 0;
			HandleButtonPress(GPIOA, GPIO_PIN_9);
		}
	}

	if (relevant_changes_a & GPIO_PIN_10) // CIRCLE
	{
		if (last_gpioa_state & GPIO_PIN_10) {
			changed_statea_10 = 1;
			HandleButtonPress(GPIOA, GPIO_PIN_10);
		} else {
			changed_statea_10 = 0;
			HandleButtonPress(GPIOA, GPIO_PIN_10);
		}
	}

	if (relevant_changes_b & GPIO_PIN_0) // SQUARE
	{
		if (last_gpiob_state & GPIO_PIN_0) {
			changed_stateb_0 = 1;
			HandleButtonPress(GPIOB, GPIO_PIN_0);
		} else {
			changed_stateb_0 = 0;
			HandleButtonPress(GPIOB, GPIO_PIN_0);
		}
	}

//---------------------------------------------------------------CHREST/TRIANGLE/CIRCLE/SQUARE END

//---------------------------------------------------------------R1/R2 START

	if (relevant_changes_a & GPIO_PIN_14) // R1
	{
		if (last_gpioa_state & GPIO_PIN_14) {
			changed_statea_14 = 1;
			HandleButtonPress(GPIOA, GPIO_PIN_14);
		} else {
			changed_statea_14 = 0;
			HandleButtonPress(GPIOA, GPIO_PIN_14);
		}
	}

	if (relevant_changes_a & GPIO_PIN_13) // R2
	{
		if (last_gpioa_state & GPIO_PIN_13) {
			changed_statea_13 = 1;
			HandleButtonPress(GPIOA, GPIO_PIN_13);
		} else {
			changed_statea_13 = 0;
			HandleButtonPress(GPIOA, GPIO_PIN_13);
		}
	}

//---------------------------------------------------------------R1/R2 END

//---------------------------------------------------------------L1/L2 START

	if (relevant_changes_a & GPIO_PIN_15) // L1
	{
		if (last_gpioa_state & GPIO_PIN_15) {
			changed_statea_15 = 1;
			HandleButtonPress(GPIOA, GPIO_PIN_15);
		} else {
			changed_statea_15 = 0;
			HandleButtonPress(GPIOA, GPIO_PIN_15);
		}
	}

	if (relevant_changes_b & GPIO_PIN_3) // L2
	{
		if (last_gpiob_state & GPIO_PIN_3) {
			changed_stateb_3 = 1;
			HandleButtonPress(GPIOB, GPIO_PIN_3);
		} else {
			changed_stateb_3 = 0;
			HandleButtonPress(GPIOB, GPIO_PIN_3);
		}
	}

//---------------------------------------------------------------L1/L2 END

//---------------------------------------------------------------LEFT/UP/RIGHT/DOWN START

	if (relevant_changes_b & GPIO_PIN_4) // LEFT
	{
		if (last_gpiob_state & GPIO_PIN_4) {
			changed_stateb_4 = 1;
			HandleButtonPress(GPIOB, GPIO_PIN_4);
		} else {
			changed_stateb_4 = 0;
			HandleButtonPress(GPIOB, GPIO_PIN_4);
		}
	}

	if (relevant_changes_b & GPIO_PIN_5) // UP
	{
		if (last_gpiob_state & GPIO_PIN_5) {
			changed_stateb_5 = 1;
			HandleButtonPress(GPIOB, GPIO_PIN_5);
		} else {
			changed_stateb_5 = 0;
			HandleButtonPress(GPIOB, GPIO_PIN_5);
		}
	}

	if (relevant_changes_b & GPIO_PIN_6) // RIGHT
	{
		if (last_gpiob_state & GPIO_PIN_6) {
			changed_stateb_6 = 1;
			HandleButtonPress(GPIOB, GPIO_PIN_6);
		} else {
			changed_stateb_6 = 0;
			HandleButtonPress(GPIOB, GPIO_PIN_6);
		}
	}

	if (relevant_changes_b & GPIO_PIN_7) // DOWN
	{
		if (last_gpiob_state & GPIO_PIN_7) {
			changed_stateb_7 = 1;
			HandleButtonPress(GPIOB, GPIO_PIN_7);
		} else {
			changed_stateb_7 = 0;
			HandleButtonPress(GPIOB, GPIO_PIN_7);
		}
	}

//---------------------------------------------------------------LEFT/UP/RIGHT/DOWN END

	previous_gpioa_state = last_gpioa_state;
	previous_gpiob_state = last_gpiob_state;
}
