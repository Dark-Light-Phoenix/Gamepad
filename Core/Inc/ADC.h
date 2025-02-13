#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "stdint.h"

#define BUFFER_SIZE 24
#define DEAD_ZONE 150
#define NEUTRAL_ZONE 2048

void ADC_DMA_Init (void);
void ADC_Sorting (uint16_t buff[BUFFER_SIZE], uint16_t buff2[2]);
void ADC_Filtering (void);
void ADC_Scale (void);

#endif /* INC_ADC_H_ */
