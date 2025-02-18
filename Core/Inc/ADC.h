#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "stdint.h"
#include "stm32g4xx_hal.h"

void ADC_DMA_Init (void);
void ADC_Filtering (void);
void ADC_Scale (void);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);

#endif /* INC_ADC_H_ */
