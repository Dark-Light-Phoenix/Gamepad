#include "ADC.h"
#include "stm32g4xx_hal.h"
#include "stdio.h"
#include "stdlib.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern TIM_HandleTypeDef htim6;

uint16_t adc_buffer1 [BUFFER_SIZE];
uint16_t adc_buffer2 [BUFFER_SIZE];

int16_t adc_x1, adc_x2;
int16_t adc_y1, adc_y2;

void ADC_DMA_Init (void)
{
	HAL_ADC_Start_DMA (&hadc1, (uint32_t *)adc_buffer1, BUFFER_SIZE);
	HAL_ADC_Start_DMA (&hadc2, (uint32_t *)adc_buffer2, BUFFER_SIZE);

	HAL_TIM_Base_Start (&htim6);
}

void ADC_Scale (void)
{
	//float cal_x1 = NEUTRAL_ZONE - adc_buffer1[0];
	float dif_x1 = adc_buffer1[0]; //+ cal_x1;

	if (dif_x1 >= NEUTRAL_ZONE - DEAD_ZONE && dif_x1 <= NEUTRAL_ZONE + DEAD_ZONE)
	{
		adc_x1 = NEUTRAL_ZONE;
	} else if (dif_x1 < NEUTRAL_ZONE - DEAD_ZONE && dif_x1 > NEUTRAL_ZONE + DEAD_ZONE) {
		if (dif_x1 >= 300 && dif_x1 <= 3800)
		{
			adc_x1 = dif_x1;
		}
	} else {
		return;
	}
}
