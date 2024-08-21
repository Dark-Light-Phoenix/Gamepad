#include "Joystick ADC.h"
#include "stm32g4xx_hal.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;

uint8_t ReadJoystickAxis (uint32_t channel, ADC_HandleTypeDef* hadc)
{
	ADC_ChannecConfTypeDef sConfig = {0};
	sConfig.Channel = channel;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
	HAL_ADC_ConfigChannel (hadc, &sConfig);

	HAL_ADC_Start (hadc);
	HAL_ADC_PollforConversion (hadc, HAL_MAX_DELAY);
	uint32_t adcValue = HAL_ADC_GetValue (hadc);
	HAL_ADC_Stop (hadc);

	return (uint8_t)((adcValue * 255 / 4095) - 128);
}
