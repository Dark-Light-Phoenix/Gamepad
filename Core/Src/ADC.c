#include "ADC.h"
#include "stm32g4xx_hal.h"
#include "stdio.h"
#include "stdlib.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern TIM_HandleTypeDef htim6;

uint16_t adc_buffer1 [BUFFER_SIZE];
uint16_t adc_buffer2 [BUFFER_SIZE];

uint16_t filter_buffer1[(BUFFER_SIZE / 2)];
uint16_t filter_buffer2[(BUFFER_SIZE / 2)];

uint16_t scaled_buffer1[2];
uint16_t scaled_buffer2[2];

int16_t adc_x1, adc_x2;
int16_t adc_y1, adc_y2;

void ADC_DMA_Init (void)
{
	HAL_ADC_Start_DMA (&hadc1, (uint32_t *)adc_buffer1, BUFFER_SIZE);
	HAL_ADC_Start_DMA (&hadc2, (uint32_t *)adc_buffer2, BUFFER_SIZE);

	HAL_TIM_Base_Start (&htim6);
}

void ADC_Sorting (uint16_t buff[BUFFER_SIZE], uint16_t buff2[2])
{
	uint16_t min_even = buff[0], max_even = buff[0];
	uint16_t min_odd = buff[1], max_odd = buff[1];
	uint32_t sum_even = 0, sum_odd = 0;

	for (uint8_t i = 0; i < BUFFER_SIZE; i++)
	{
		if (i % 2 == 0) {
			if (buff[i] < min_even) {
				min_even = buff[i];
			} else if (buff[i] > max_even) {
				max_even = buff[i];
			}
		} else {
			if (buff[i] < min_odd) {
				min_odd = buff[i];
			} else if (buff[i] > max_odd) {
				max_odd = buff[i];
			}
		}
	}

	for (uint8_t i = 0; i < BUFFER_SIZE; i++)
	{
		if (i % 2 == 0) {
			if (buff[i] != min_even && buff[i] != max_even) {
				sum_even += buff[i];
			}
		} else {
			if (buff[i] != min_odd && buff[i] != max_odd) {
				sum_odd += buff[i];
			}
		}
	}

	buff2[0] = sum_even / (BUFFER_SIZE / 2);
	buff2[1] = sum_odd / (BUFFER_SIZE / 2);
}

void ADC_Filtering (void)
{
	ADC_Sorting (adc_buffer1, scaled_buffer1);
	ADC_Sorting (adc_buffer2, scaled_buffer2);
}

void ADC_Scale (void)
{
	ADC_Filtering();
}
