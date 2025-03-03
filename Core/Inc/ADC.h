#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "stdint.h"
#include "stm32g4xx_hal.h"

#define MID_ZONE 2048
#define DEAD_ZONE 150
#define TRESHOLD 3500

void ADC_DMA_Init (void);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);
void Update_Range (void);
void ADC_Filtering (uint16_t value, volatile uint16_t min_value, volatile uint16_t max_value, int16_t *adc_value);
void ADC_Scale (void);

#endif /* INC_ADC_H_ */
