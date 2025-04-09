#include "Buttons and Functions.h"
#include "stm32g4xx_it.h"
#include "stm32g4xx_hal.h"
#include "stdlib.h"
#include "math.h"
#include "stdbool.h"
#include "main.h"

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim16;

ButtonState CurrentState = STATIC;

extern volatile uint16_t last_gpioa_state;
extern volatile uint16_t last_gpiob_state;
extern volatile uint16_t previous_gpioa_state;
extern volatile uint16_t previous_gpiob_state;

volatile bool press = false;
volatile uint32_t overflow_count = 0;

uint8_t changed_statea_3 = 1;
uint8_t changed_statea_5 = 1;
uint8_t changed_statea_8 = 1;
uint8_t changed_statea_9 = 1;
uint8_t changed_statea_10 = 1;
uint8_t changed_statea_13 = 1;
uint8_t changed_statea_14 = 1;
uint8_t changed_statea_15 = 1;

uint8_t changed_stateb_0 = 1;
uint8_t changed_stateb_3 = 1;
uint8_t changed_stateb_4 = 1;
uint8_t changed_stateb_5 = 1;
uint8_t changed_stateb_6 = 1;
uint8_t changed_stateb_7 = 1;

uint8_t changed_stateg_10 = 0;

static uint8_t check_status = 0;
static uint8_t status_flag1 = 0;
static uint8_t status_flag2 = 0;
static uint8_t light_on = 0;
uint32_t color = 0;

uint32_t LED_Data[NUM_LEDS][3] = {0}; // for color set
uint32_t pwmData [NUM_LEDS][(LED_BITS)] = {0}; // Buffer for LED data
uint32_t dmaBuff [DMA_BUFFER] = {0};

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
	}

	if (htim -> Instance == TIM17)
	{
		if (HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_3) == GPIO_PIN_SET) {
			changed_statea_3 = 1;
			if (!status_flag1) HandleButtonPress (GPIOA, GPIO_PIN_3);
			status_flag1 = 1;
		} else {
			changed_statea_3 = 0;
			if (status_flag1) HandleButtonPress (GPIOA, GPIO_PIN_3);
			status_flag1 = 0;
		}

		if (HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_5) == GPIO_PIN_SET) {
			changed_statea_5 = 1;
			if (!status_flag2) HandleButtonPress (GPIOA, GPIO_PIN_5);
			status_flag2 = 1;
			if (CurrentState == SHORT)
			{

			} else if (CurrentState == LONG)
			{
				if (light_on)
				{
					BacklightOff();
					light_on = 0;
					CurrentState = STATIC;
				} else {
					BacklightOn();
					light_on = 1;
					CurrentState = STATIC;
				}
			}
		} else {
			changed_statea_5 = 0;
			if (status_flag2) HandleButtonPress (GPIOA, GPIO_PIN_5);
			status_flag2 = 0;
		}
	}
}

void ButtonStatus (void)
{
	last_gpioa_state = GPIOA->IDR & GPIOA_MASK;
	last_gpiob_state = GPIOB->IDR & GPIOB_MASK;

	uint32_t changed_pins_a = last_gpioa_state ^ previous_gpioa_state;
	uint32_t relevant_changes_a = changed_pins_a & GPIOA_MASK;
	uint32_t changed_pins_b = last_gpiob_state ^ previous_gpiob_state;
	uint32_t relevant_changes_b = changed_pins_b & GPIOB_MASK;

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

	if (relevant_changes_b & GPIO_PIN_4) // RIGHT
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

	if (relevant_changes_b & GPIO_PIN_6) // DOWN
	{
		if (last_gpiob_state & GPIO_PIN_6) {
			changed_stateb_6 = 1;
			HandleButtonPress(GPIOB, GPIO_PIN_6);
		} else {
			changed_stateb_6 = 0;
			HandleButtonPress(GPIOB, GPIO_PIN_6);
		}
	}

	if (relevant_changes_b & GPIO_PIN_7) // LEFT
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

void Prepare_Data (uint8_t indx)
{
	color = ((LED_Data[indx][0] << 16) | (LED_Data[indx][1] << 8) | (LED_Data[indx][2]));

	for (int i = 23; i >= 0; i--)
	{
		if (((color >> i) & 1) == 1)
		{
			pwmData [indx][i] = PWM_HIGH;
		} else {
			pwmData [indx][i] = PWM_LOW;
		}
	}
}

void Set_DMA_Buffer (void)
{
	  for (uint8_t led = 0; led < NUM_LEDS; led++)
	  {
		  for (uint8_t bit = 0; bit < LED_BITS; bit++) {
			  dmaBuff [led * LED_BITS + bit] = pwmData [led][bit];
		  }
	  }
}

void Set_Color (uint8_t indx, uint8_t green, uint8_t red, uint8_t blue)
{
  LED_Data[indx][0] = green;
  LED_Data[indx][1] = red;
  LED_Data[indx][2] = blue;

  Prepare_Data(indx);
  Set_DMA_Buffer();
}

void BacklightOff (void)
{
	for (int i = 0; i < NUM_LEDS; i++)
	{
		Set_Color (i, 0, 0, 0);
	}

	Start_PWM();
}

void BacklightOn (void)
{
	for (int i = 0; i < NUM_LEDS; i++)
	{
		Set_Color (i, 255, 255, 0);
	}

	Start_PWM();
}

void Start_PWM (void)
{
	  HAL_TIM_PWM_Start_DMA (&htim3, TIM_CHANNEL_2, (uint32_t*) dmaBuff, (DMA_BUFFER * 2));
}
