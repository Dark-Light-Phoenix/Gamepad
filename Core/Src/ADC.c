#include "ADC.h"
#include "stm32g4xx_hal.h"
#include "stdio.h"
#include "stdlib.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern TIM_HandleTypeDef htim6;

volatile uint16_t min_even1_val = 4096, mid_even1_val = 0, max_even1_val = 0;
volatile uint16_t min_odd1_val = 4096, mid_odd1_val = 0, max_odd1_val = 0;

volatile uint16_t min_even2_val = 4096, mid_even2_val = 0, max_even2_val = 0;
volatile uint16_t min_odd2_val = 4096, mid_odd2_val = 0, max_odd2_val = 0;

volatile uint16_t adc_buffer1[2];
volatile uint16_t adc_buffer2[2];

uint16_t filter_buff1[6];
uint16_t filter_buuf2[6];

int16_t adc_x1, adc_x2;
int16_t adc_y1, adc_y2;

void ADC_DMA_Init (void)
{
	HAL_ADC_Start_DMA (&hadc1, (uint32_t *)adc_buffer1, 2);
	HAL_ADC_Start_DMA (&hadc2, (uint32_t *)adc_buffer2, 2);

	HAL_TIM_Base_Start (&htim6);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	if (hadc->Instance == ADC1);
	ADC_Scale();
	if (hadc->Instance == ADC2);
	ADC_Scale();
}

void ADC_Filtering (void)
{
	uint16_t x1_value = adc_buffer1[0];
	uint16_t y1_value = adc_buffer1[1];
	uint16_t x2_value = adc_buffer2[0];
	uint16_t y2_value = adc_buffer2[1];

	if (x1_value < min_even1_val) min_even1_val = x1_value;
	if (x1_value > max_even1_val) max_even1_val = x1_value;

	if (y1_value < min_odd1_val) min_odd1_val = y1_value;
	if (y1_value > max_odd1_val) max_odd1_val = y1_value;

	if (x2_value < min_even2_val) min_even2_val = x2_value;
	if (x2_value > max_even2_val) max_even2_val = x2_value;

	if (y2_value < min_odd2_val) min_odd2_val = y2_value;
	if (y2_value > max_odd2_val) max_odd2_val = y2_value;
}

void ADC_Scale (void)
{
	ADC_Filtering();
}
