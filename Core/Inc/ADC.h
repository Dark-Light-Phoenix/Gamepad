#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "stdint.h"

#define BUFFER_SIZE 100

void ADC_DMA_Init (void);
void ADC_Sorting (uint16_t buff[BUFFER_SIZE], uint16_t buff2[2]);
void ADC_Filtering (void);
void ADC_Scale (void);
void ADC_Calibration (uint16_t buff[BUFFER_SIZE], uint16_t buff2[]);

#endif /* INC_ADC_H_ */
