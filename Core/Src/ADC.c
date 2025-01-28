#include "ADC.h"
#include "stm32g4xx_hal.h"
#include "stdio.h"
#include "stdlib.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern TIM_HandleTypeDef htim6;

uint16_t adc_buffer1 [BUFFER_SIZE];
uint16_t adc_buffer2 [BUFFER_SIZE];

int8_t adc_x1, adc_x2;
int8_t adc_y1, adc_y2;

void ADC_DMA_Init (void)
{
	HAL_ADC_Start_DMA (&hadc1, (uint32_t *)adc_buffer1, BUFFER_SIZE);
	HAL_ADC_Start_DMA (&hadc2, (uint32_t *)adc_buffer2, BUFFER_SIZE);

	HAL_TIM_Base_Start (&htim6);

	adc_x1 = adc_buffer1[0];
	adc_y1 = adc_buffer1[1];

	adc_x2 = adc_buffer2[0];
	adc_y2 = adc_buffer2[1];
}
