#include "RGB.h"
#include "stm32g4xx_it.h"
#include "stm32g4xx_hal.h"
#include "stdlib.h"
#include "math.h"

extern DMA_HandleTypeDef hdma_tim17_ch1;
extern TIM_HandleTypeDef htim17;
extern TIM_HandleTypeDef htim7;

uint8_t LED_Data[NUM_LEDS][3]; // for color set
uint16_t pwmData [(LED_BITS * NUM_LEDS) + RES]; // Buffer for LED data

volatile uint32_t delayFlag = 0;
volatile uint8_t breathing_phase = 0;
volatile uint8_t brightness = 0;

uint8_t max_brightness = 45;
uint32_t breathing_delay = 500;
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

		for (uint8_t j = 23; j >= 0; j--)
		{
			pwmData[indx++] = (color & (1 << j) ? PWM_HIGH : PWM_LOW);
		}
	}

	for (uint8_t i = 0; i < RES; i++)
	{
		pwmData[indx++] = 0; // Reset signal
	}
}

void Send_Data (void)
{
	Prepare_Data();
	HAL_TIM_PWM_Start_DMA (&htim17, TIM_CHANNEL_1, (uint32_t*) pwmData, sizeof(pwmData) / sizeof(uint16_t));
	while (!delayFlag){};
	delayFlag = 0;
}

void HAL_TIM_PWM_PulseFinishedCallback (TIM_HandleTypeDef *htim) // Function of stopping DMA after completion of sending
{
	if (htim -> Instance == TIM17)
	{
		HAL_TIM_PWM_Stop_DMA (&htim17, TIM_CHANNEL_1);
		delayFlag = 1;
	}
}

void Breathing_Delay (uint32_t delay_ms)
{
	uint32_t arr_value = (delay_ms * 1000) - 1;
	__HAL_TIM_SET_COUNTER (&htim7, 0);
	htim7.Instance -> ARR = arr_value;
	HAL_TIM_Base_Start_IT (&htim7);
}

void Breathing (void)
{
	switch (breathing_phase)
	{
	case 0:
		if (breathing_phase <= max_brightness)
		{
			for (uint8_t i = 0; i < NUM_LEDS; i++)
		{
				float scale = (float)brightness / max_brightness;
				Set_Color (i, 255 * scale, 255 * scale, 255 * scale);
		}
			Send_Data();
			brightness++;
		}
		else
		{
			breathing_phase = 1;
			Breathing_Delay (breathing_delay);
		} break;
	case 1:
		break;
	case 2:
		if (brightness > 0)
		{
			for (uint8_t i = 0; i < NUM_LEDS; i++)
		{
				float scale = (float)brightness / max_brightness;
				Set_Color (i, 255 * scale, 255 * scale, 255 * scale);
		}
			Send_Data();
			brightness--;
		}
		else
		{
			breathing_phase = 1;
			Breathing_Delay (breathing_delay);
		}
		break;
	}
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
}

