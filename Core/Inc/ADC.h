#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "stdint.h"
#include "stm32g4xx_hal.h"

#define MID_ZONE 2048
#define DEAD_ZONE 150

void ADC_DMA_Init (void);
void ADC_Filtering (uint16_t x1_value, uint16_t y1_value, uint16_t x2_value, uint16_t y2_value);
void ADC_Scale (void);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);

#endif /* INC_ADC_H_ */
