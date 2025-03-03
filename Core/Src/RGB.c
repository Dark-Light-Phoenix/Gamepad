#include "RGB.h"
#include "stm32g4xx_it.h"
#include "stm32g4xx_hal.h"
#include "stdlib.h"
#include "math.h"
#include "stdbool.h"

extern TIM_HandleTypeDef htim17;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim7;

uint32_t release_time = 0;
volatile uint32_t overflow_count = 0;
ButtonState CurrentState = STATIC;

extern volatile uint16_t last_gpioa_state;
extern volatile uint16_t last_gpiob_state;
extern volatile uint16_t last_gpiog_state;
extern volatile uint16_t previous_gpioa_state;
extern volatile uint16_t previous_gpiob_state;
extern volatile uint16_t previous_gpiog_state;

void HandleButtonPress (GPIO_TypeDef *GPIO, uint16_t GPIO_Pin)
{
	if (HAL_GPIO_ReadPin (GPIO, GPIO_Pin) == GPIO_PIN_SET)
	{
		__HAL_TIM_SET_COUNTER(&htim16, 0);
		overflow_count = 0;
	}

	if (HAL_GPIO_ReadPin (GPIO, GPIO_Pin) == GPIO_PIN_RESET)
	{
		release_time = __HAL_TIM_GET_COUNTER (&htim16) + (overflow_count * 100);
	}
}

extern uint8_t counter;
extern uint8_t delay;
volatile bool press = false;

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

void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim)
{
	if (htim -> Instance == TIM16)
	{
		overflow_count++;
	}

	if (htim -> Instance == TIM7)
	{
		volatile uint16_t last_gpio_statea = 0;
		volatile uint16_t previous_gpio_statea = 0;

		uint16_t changed_pins_a = last_gpio_statea ^ previous_gpio_statea;
		uint16_t relevant_changes_a = changed_pins_a & GPIO_INPUT_MASK_A;
		uint16_t changed_pins_g = last_gpiog_state ^ previous_gpiog_state;
		uint16_t relevant_changes_g = changed_pins_g & GPIO_INPUT_MASK_G;

		counter += 1;

		last_gpioa_state = GPIOA->IDR & GPIO_INPUT_MASK_A;
		last_gpiog_state = GPIOG->IDR & GPIO_INPUT_MASK_G;

		if (relevant_changes_a & GPIO_PIN_3)
		{
			if (last_gpioa_state & GPIO_PIN_3) {
				changed_statea_3 = 1;
			} else {
				changed_statea_3 = 0;
			}
		}

		if (relevant_changes_g & GPIO_PIN_10)
		{
			if (last_gpiog_state & GPIO_PIN_10) {
				changed_stateg_10 = 1;
			} else {
				changed_stateg_10 = 0;
			}
		}
	}
}

void Counter (void)
{
	counter += 1;

	uint16_t changed_pins_a = last_gpioa_state ^ previous_gpioa_state;
	uint16_t relevant_changes_a = changed_pins_a & GPIOA_MASK;
	uint16_t changed_pins_b = last_gpiob_state ^ previous_gpiob_state;
	uint16_t relevant_changes_b = changed_pins_b & GPIOB_MASK;

	if (relevant_changes_a & GPIO_PIN_5)
	{
		if (last_gpioa_state & GPIO_PIN_5) {
			changed_statea_5 = 1;
		} else {
			changed_statea_5 = 0;
		}
	}

	if (relevant_changes_a & GPIO_PIN_8)
	{
		if (last_gpioa_state & GPIO_PIN_8) {
			changed_statea_8 = 1;
		} else {
			changed_statea_8 = 0;
		}
	}

	if (relevant_changes_a & GPIO_PIN_9)
	{
		if (last_gpioa_state & GPIO_PIN_9) {
			changed_statea_9 = 1;
		} else {
			changed_statea_9 = 0;
		}
	}

	if (relevant_changes_a & GPIO_PIN_10)
	{
		if (last_gpioa_state & GPIO_PIN_10) {
			changed_statea_10 = 1;
		} else {
			changed_statea_10 = 0;
		}
	}

	if (relevant_changes_a & GPIO_PIN_13)
	{
		if (last_gpioa_state & GPIO_PIN_13) {
			changed_statea_13 = 1;
		} else {
			changed_statea_13 = 0;
		}
	}

	if (relevant_changes_a & GPIO_PIN_14)
	{
		if (last_gpioa_state & GPIO_PIN_14) {
			changed_statea_14 = 1;
		} else {
			changed_statea_14 = 0;
		}
	}

	if (relevant_changes_a & GPIO_PIN_15)
	{
		if (last_gpioa_state & GPIO_PIN_14) {
			changed_statea_15 = 1;
		} else {
			changed_statea_15 = 0;
		}
	}

	if (relevant_changes_b & GPIO_PIN_0)
	{
		if (last_gpiob_state & GPIO_PIN_0) {
			changed_stateb_0 = 1;
		} else {
			changed_stateb_0 = 0;
		}
	}


	if (relevant_changes_b & GPIO_PIN_3)
	{
		if (last_gpiob_state & GPIO_PIN_3) {
			changed_stateb_3 = 1;
		} else {
			changed_stateb_3 = 0;
		}
	}

	if (relevant_changes_b & GPIO_PIN_4)
	{
		if (last_gpiob_state & GPIO_PIN_4) {
			changed_stateb_4 = 1;
		} else {
			changed_stateb_4 = 0;
		}
	}

	if (relevant_changes_b & GPIO_PIN_5)
	{
		if (last_gpiob_state & GPIO_PIN_5) {
			changed_stateb_5 = 1;
		} else {
			changed_stateb_5 = 0;
		}
	}

	if (relevant_changes_b & GPIO_PIN_6)
	{
		if (last_gpiob_state & GPIO_PIN_6) {
			changed_stateb_6 = 1;
		} else {
			changed_stateb_6 = 0;
		}
	}

	if (relevant_changes_b & GPIO_PIN_7)
	{
		if (last_gpiob_state & GPIO_PIN_7) {
			changed_stateb_7 = 1;
		} else {
			changed_stateb_7 = 0;
		}
	}

	previous_gpioa_state = last_gpioa_state;
	previous_gpiob_state = last_gpiob_state;
//	if (!press)
//	{
//		__HAL_TIM_SET_COUNTER (&htim16, 0);
//		press = true;
//		overflow_count = 0;
//	} else
//	{
//		delay = __HAL_TIM_GET_COUNTER (&htim16) + (overflow_count * 1000);
//		press = false;
//	}
}

//uint8_t LED_Data[NUM_LEDS][3]; // for color set
//uint32_t pwmData [(LED_BITS * NUM_LEDS) + (RES * NUM_LEDS)]; // Buffer for LED data
//
//volatile uint32_t delayFlag = 0;
//volatile uint8_t brightness = 0;
//
//uint8_t gradient_position = 0;
//
//void Set_Color (uint8_t index, uint8_t green, uint8_t red, uint8_t blue)
//{
//  LED_Data[index][0] = green;
//  LED_Data[index][1] = red;
//  LED_Data[index][2] = blue;
//}
//
//void Prepare_Data (void)
//{
//  uint32_t indx = 0;
//
//  for (uint8_t i = 0; i < NUM_LEDS; i++)
//  {
//    uint32_t color = ((LED_Data[i][0] << 16) | (LED_Data[i][1] << 8) | (LED_Data[i][2]));
//
//    for (uint8_t j = 24; j > 0; j--)
//    {
//      pwmData[indx++] = (color & (1 << j)) ? PWM_HIGH : PWM_LOW;
//    }
//
//    for (uint16_t k = 0; k < RES; k++)
//    {
//      pwmData[indx++] = 0; // Reset signal
//    }
//  }
//}
//
//void Send_Data (void)
//{
//  Prepare_Data();
//  HAL_TIM_PWM_Start_DMA (&htim17, TIM_CHANNEL_1, (uint32_t*) pwmData, sizeof(pwmData));
//}
//
//void Breathing (void)
//{
//  for (uint8_t brightness = 0; brightness < 256; brightness++)
//  {
//    Set_Color (0, brightness, 0, 0);
//    Set_Color (1, 0, brightness, 0);
//    Set_Color (3, 0, 0, brightness);
//    Set_Color (4, brightness, 0, brightness);
//    Set_Color (5, brightness, brightness, 0);
//    Set_Color (6, 0, brightness, brightness);
//    Set_Color (7, (int)(brightness / 3), (int)(brightness / 5), (int)(brightness / 1));
//    Set_Color (8, (int)(brightness / 6), (int)(brightness / 2), (int)(brightness / 9));
//  }
//
//  Send_Data();
//
//  HAL_TIM_Base_Start_IT (&htim16);
//
//    for (uint8_t brightness = 255; brightness >= 9; brightness--)
//    {
//      Set_Color (0, brightness, 0, 0);
//      Set_Color (1, 0, brightness, 0);
//      Set_Color (3, 0, 0, brightness);
//      Set_Color (4, brightness, 0, brightness);
//      Set_Color (5, brightness, brightness, 0);
//      Set_Color (6, 0, brightness, brightness);
//      Set_Color (7, brightness / 3, brightness / 5, brightness);
//      Set_Color (8, brightness / 6, brightness / 2, brightness / 9);
//    }
//    Send_Data();
//
//  HAL_TIM_Base_Start_IT (&htim16);
//}
//
//void Gradient (uint8_t position, uint8_t* green, uint8_t* red, uint8_t* blue)
//{
//  uint8_t section = position / 43;
//  uint8_t offset = position % 43;
//  uint8_t increment = 255 / 43;
//
//  switch (section)
//  {
//  case 0: *green = offset * increment; *red = 255; *blue = 0; break; // RED to YELLOW
//  case 1: *green = 255; *red = 255 - offset * increment; *blue = 0; break; // YELLOW to GREEN
//  case 2: *green = 255; *red = 0; *blue = offset * increment; break; // GREEN to CYAN
//  case 3: *green = 255 - offset * increment; *red = 0; *blue = 255; break; // CYAN to BLUE
//  case 4: *green = 0; *red = offset * increment; *blue = 255; break; // BLUE to MAGENTA
//  case 5: *green = 0; *red = 255; *blue = 255 - offset * increment; break; // MAGENTA to RED
//  }
//}
//
//void Update_Gradient (void)
//{
//  for (uint8_t i = 0; i < NUM_LEDS; i++)
//  {
//    uint8_t red, green, blue;
//    uint8_t step = (gradient_position + (i * (GRADIENT_STEPS / NUM_LEDS))) % GRADIENT_STEPS;
//    Gradient (step, &green, &red, &blue);
//
//    float brightness_factor = 1 - (0.2 * (i % 2)); // brightness scaling
//    LED_Data[i][0] = green * brightness_factor;
//    LED_Data[i][1] = red * brightness_factor;
//    LED_Data[i][2] = blue * brightness_factor;
//  }
//  gradient_position = (gradient_position + 1) % GRADIENT_STEPS;
//
//  Send_Data();
//}
//
//void UpdateLightMode (void)
//{
//  switch (currentLightMode)
//  {
//  case LIGHT_MODE_BREATHING:
//    Breathing();
//    break;
//  case LIGHT_MODE_GRADIENT:
//    Update_Gradient();
//    break;
//  default:
//    break;
//  }
//}
//
//void TurnLightOff (void)
//{
//  for (uint8_t i = 0; i < NUM_LEDS; i++)
//  {
//    Set_Color (i, 0, 0, 0);
//  }
//
//  Send_Data();
//}
//
//void TurnLightOn (void)
//{
//  for (uint8_t i = 0; i < NUM_LEDS; i++)
//  {
//    Set_Color (i, 255, 255, 255);
//  }
//
//  Send_Data();
//}
//
//void HAL_TIM_PWM_PulseFinishedCallback (TIM_HandleTypeDef *htim) // Function of stopping DMA after completion of sending
//{
//  if (htim -> Instance == TIM17)
//  {
//    HAL_TIM_PWM_Stop_DMA (&htim17, TIM_CHANNEL_1);
//  }
//}
