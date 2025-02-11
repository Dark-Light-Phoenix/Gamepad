#ifndef INC_ADC_H_
#define INC_ADC_H_

#define BUFFER_SIZE 2
#define DEAD_ZONE 150
#define NEUTRAL_ZONE 2048

void ADC_DMA_Init (void);
void ADC_Scale (void);

#endif /* INC_ADC_H_ */
