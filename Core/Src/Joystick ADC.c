#include "Joystick ADC.h"
#include "stm32g4xx_hal.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;

void ADC_DMA_Init (void)
{
	HAL_ADC_Start_DMA (&hadc1, (uint32_t *)adc_buffer1, BUFFER_SIZE);
	HAL_ADC_Start_DMA (&hadc2, (uint32_t *)adc_buffer2, BUFFER_SIZE);

	HAL_TIM_Base_Start (&htim6);
}
