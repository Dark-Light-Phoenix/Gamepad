#include "RGB.h"
#include "stm32g4xx_it.h"
#include "stm32g4xx_hal.h"
#include "stdlib.h"
#include "math.h"

extern DMA_HandleTypeDef hdma_tim17_ch1;
extern TIM_HandleTypeDef htim17;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim7;

uint8_t LED_Data[NUM_LEDS][3]; // for color set
uint32_t pwmData [(LED_BITS * NUM_LEDS) + (RES * NUM_LEDS)]; // Buffer for LED data

volatile uint32_t delayFlag = 0;
volatile uint8_t brightness = 0;

uint8_t gradient_position = 0;

void Set_Color (uint8_t index, uint8_t green, uint8_t red, uint8_t blue)
{
	LED_Data[index][0] = green;
	LED_Data[index][1] = red;
	LED_Data[index][2] = blue;
}

void Prepare_Data (void)
{
	uint32_t indx = 0;

	for (uint8_t i = 0; i < NUM_LEDS; i++)
	{
		uint32_t color = ((LED_Data[i][0] << 16) | (LED_Data[i][1] << 8) | (LED_Data[i][2]));

		for (uint8_t j = 24; j > 0; j--)
		{
			pwmData[indx++] = (color & (1 << j)) ? PWM_HIGH : PWM_LOW;
		}

		for (uint16_t k = 0; k < RES; k++)
		{
			pwmData[indx++] = 0; // Reset signal
		}
	}
}

void Send_Data (void)
{
	Prepare_Data();
	HAL_TIM_PWM_Start_DMA (&htim17, TIM_CHANNEL_1, (uint32_t*) pwmData, sizeof(pwmData));
}

void HAL_TIM_PWM_PulseFinishedCallback (TIM_HandleTypeDef *htim) // Function of stopping DMA after completion of sending
{
	if (htim -> Instance == TIM17)
	{
		HAL_TIM_PWM_Stop_DMA (&htim17, TIM_CHANNEL_1);
	}
}

void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim)
{
	if (htim -> Instance == TIM7)
	{
		HAL_TIM_Base_Stop_IT (&htim7);
	}
}

void Breathing (void)
{
	for (uint8_t brightness = 0; brightness < 256; brightness++)
	{
		Set_Color (0, brightness, 0, 0);
		Set_Color (1, 0, brightness, 0);
		Set_Color (3, 0, 0, brightness);
		Set_Color (4, brightness, 0, brightness);
		Set_Color (5, brightness, brightness, 0);
		Set_Color (6, 0, brightness, brightness);
		Set_Color (7, (int)(brightness / 3), (int)(brightness / 5), (int)(brightness / 1));
		Set_Color (8, (int)(brightness / 6), (int)(brightness / 2), (int)(brightness / 9));
	}

	Send_Data();

	HAL_TIM_Base_Start_IT (&htim7);

		for (uint8_t brightness = 255; brightness >= 9; brightness--)
		{
			Set_Color (0, brightness, 0, 0);
			Set_Color (1, 0, brightness, 0);
			Set_Color (3, 0, 0, brightness);
			Set_Color (4, brightness, 0, brightness);
			Set_Color (5, brightness, brightness, 0);
			Set_Color (6, 0, brightness, brightness);
			Set_Color (7, brightness / 3, brightness / 5, brightness);
			Set_Color (8, brightness / 6, brightness / 2, brightness / 9);
		}
		Send_Data();

	HAL_TIM_Base_Start_IT (&htim7);
}

void Gradient (uint8_t position, uint8_t* green, uint8_t* red, uint8_t* blue)
{
	uint8_t section = position / 43;
	uint8_t offset = position % 43;
	uint8_t increment = 255 / 43;

	switch (section)
	{
	case 0: *green = offset * increment; *red = 255; *blue = 0; break; // RED to YELLOW
	case 1: *green = 255; *red = 255 - offset * increment; *blue = 0; break; // YELLOW to GREEN
	case 2: *green = 255; *red = 0; *blue = offset * increment; break; // GREEN to CYAN
	case 3: *green = 255 - offset * increment; *red = 0; *blue = 255; break; // CYAN to BLUE
	case 4: *green = 0; *red = offset * increment; *blue = 255; break; // BLUE to MAGENTA
	case 5: *green = 0; *red = 255; *blue = 255 - offset * increment; break; // MAGENTA to RED
	}
}

void Update_Gradient (void)
{
	for (uint8_t i = 0; i < NUM_LEDS; i++)
	{
		uint8_t red, green, blue;
		uint8_t step = (gradient_position + (i * (GRADIENT_STEPS / NUM_LEDS))) % GRADIENT_STEPS;
		Gradient (step, &green, &red, &blue);

		float brightness_factor = 1 - (0.2 * (i % 2)); // brightness scaling
		LED_Data[i][0] = green * brightness_factor;
		LED_Data[i][1] = red * brightness_factor;
		LED_Data[i][2] = blue * brightness_factor;
	}
	gradient_position = (gradient_position + 1) % GRADIENT_STEPS;

	Send_Data();
}

volatile LightState currentLightState = LIGHT_STATE_OFF;
volatile LightMode currentLightMode = LIGHT_MODE_BREATHING;

void HandleButtonPress (void)
{
	uint32_t current_time = 0;
	uint32_t released_time = 0;

	if (HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_5) == 1)
	{
		current_time = __HAL_TIM_GET_COUNTER (&htim16);
		HAL_TIM_Base_Start_IT (&htim16);

		if (HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_5) != 0)
		{
			released_time = __HAL_TIM_GET_COUNTER (&htim16);
			HAL_TIM_Base_Stop_IT (&htim16);
			__HAL_TIM_SET_COUNTER (&htim16, 0);

			if ((released_time - current_time) < 20)
			{
				current_time = 0;
				released_time = 0;
				return;
			}

			if ((released_time - current_time) > 20 && (released_time - current_time) < 200)
			{
				if (currentLightState == LIGHT_STATE_ON)
				{
					UpdateLightMode();
				} else
				{
					currentLightState = LIGHT_STATE_OFF;
					current_time = 0;
					released_time = 0;
					TurnLightOff();
				}
			} else if ((released_time - current_time) > 200 && (released_time - current_time) < 500)
			{
				currentLightState = LIGHT_STATE_ON;
				current_time = 0;
				released_time = 0;
				TurnLightOn();
			}
		} else
		{
			return;
		}
	}
}

void UpdateLightMode (void)
{
	switch (currentLightMode)
	{
	case LIGHT_MODE_BREATHING:
		Breathing();
		break;
	case LIGHT_MODE_GRADIENT:
		Update_Gradient();
		break;
	default:
		break;
	}
}

void TurnLightOff (void)
{
	for (uint8_t i = 0; i < NUM_LEDS; i++)
	{
		Set_Color (i, 0, 0, 0);
	}

	Send_Data();
}

void TurnLightOn (void)
{
	for (uint8_t i = 0; i < NUM_LEDS; i++)
	{
		Set_Color (i, 255, 255, 255);
	}

	Send_Data();
}
