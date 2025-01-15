#include "RGB.h"
#include "stm32g4xx_it.h"
#include "stm32g4xx_hal.h"
#include "stdlib.h"
#include "math.h"

extern DMA_HandleTypeDef hdma_tim17_ch1;
extern TIM_HandleTypeDef htim17;
extern TIM_HandleTypeDef htim7;

uint8_t LED_Data[NUM_LEDS][3]; // for color set
uint8_t LED_Bright[NUM_LEDS][4]; // for brightness

volatile uint32_t delayFlag = 0;

void Set_Color (uint8_t index, uint8_t green, uint8_t red, uint8_t blue)
{
	LED_Data[LedIndex][0] = green;
	LED_Data[LedIndex][1] = red;
	LED_Data[LedIndex][2] = blue;
}

void Prepare_Data (void)
{
	uint32_t indx = 0;

	for (uint8_t i = 0; i < NUM_LEDS; i++)
	{
		uint32_t color = ((LED_Bright[i][0] << 16) | (LED_Bright[i][1] << 8) | (LED_Bright[i][2]));

		for (uint8_t i = 23; i >= 0; i--)
		{
			pwmData[indx++] = (color & (1 << bit) ? PWM_HIGH : PWM_LOW);
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
		DataFlag = 1;
	}
}

void TIM7_IRQHandler (void)
{
	if (__HAL_TIM_GET_FLAG (&htim7, TIM_FLAG_UPDATE))
	{
		__HAL_TIM_CLEAR_FLAG (&htim7, TIM_FLAG_UPDATE);
		HAL_TIM_Base_Stop_IT (&htim7);
		ResetFlag = 1;
	}
}

void Breathing (void)
{
	for (uint8_t brightness = 0; brightness < 46; brightness++)
	{
		for (uint8_t i = 0; i < NUM_LEDS; i++)
		{
			float scale = brightness / 45;
			Set_Color(i, 255 * scale, 255 * scale, 255 * scale);
		}
		Send_Data();
	}
}

