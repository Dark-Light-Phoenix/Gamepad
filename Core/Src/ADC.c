#include "ADC.h"
#include "stm32g4xx_hal.h"
#include "stdio.h"
#include "stdlib.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern TIM_HandleTypeDef htim6;

uint16_t adc_buffer1 [BUFFER_SIZE];
uint16_t adc_buffer2 [BUFFER_SIZE];

uint16_t delta_buffer [DELTA_SIZE] = {120, 121, 122, 123, 124, 125};

uint16_t compensated_buffer1 [BUFFER_SIZE];
uint16_t compensated_buffer2 [BUFFER_SIZE];

int8_t adc_x1, adc_x2;
int8_t adc_y1, adc_y2;

void ADC_DMA_Init (void)
{
	HAL_ADC_Start_DMA (&hadc1, (uint32_t *)adc_buffer1, BUFFER_SIZE);
	HAL_ADC_Start_DMA (&hadc2, (uint32_t *)adc_buffer2, BUFFER_SIZE);

	HAL_TIM_Base_Start (&htim6);
}

void ScaleData (void)
{
	uint8_t neutral = 1;
	uint16_t calibration_X1 = 0;
	uint16_t calibration_Y1 = 0;
	uint16_t calibration_X2 = 0;
	uint16_t calibration_Y2 = 0;

	for (uint8_t i = 0; i < DELTA_SIZE; i++)
	{
		calibration_X1 = adc_buffer1 [0] + delta_buffer [i];
		calibration_Y1 = adc_buffer1 [1] + delta_buffer [i];

		if (calibration_X1 == 2048)
		{
			compensated_buffer1 [0] = calibration_X1;
		}

		if (calibration_Y1 == 2048)
		{
			compensated_buffer1 [1] = calibration_Y1;
		}


		if (i == DELTA_SIZE - 1)
		{
			neutral = 0;
			break;
		}
	}

	for (uint8_t i = 0; i < DELTA_SIZE; i++)
	{
		calibration_X2 = adc_buffer2 [0] + delta_buffer [i];
		calibration_Y2 = adc_buffer2 [1] + delta_buffer [i];

		if (calibration_X2 == 2048)
		{
			compensated_buffer2 [0] = calibration_X2;
		}

		if (calibration_Y2 == 2048)
		{
			compensated_buffer2 [1] = calibration_Y2;
		}

		if (i == DELTA_SIZE - 1)
		{
			neutral = 0;
			break;
		}
	}


	if (neutral)
	{
		adc_x1 = 0;
		adc_y1 = 0;
		adc_x2 = 0;
		adc_y2 = 0;
	} else
	{
		adc_x1 = (int8_t)(((compensated_buffer1 [0] * 510) / 4095) - 255);
		adc_y1 = (int8_t)(((compensated_buffer1 [1] * 510) / 4095) - 255);
		adc_x2 = (int8_t)(((compensated_buffer2 [0] * 510) / 4095) - 255);
		adc_y2 = (int8_t)(((compensated_buffer2 [1] * 510) / 4095) - 255);
	}
}
