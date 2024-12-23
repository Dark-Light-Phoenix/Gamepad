#ifndef INC_JOYSTICK_ADC_H_
#define INC_JOYSTICK_ADC_H_

#define BUFFER_SIZE 2
#define DELTA_SIZE 6

void ADC_DMA_Init (void);
void Calibrate_Joystics (void);
void Filter_Data (void);
void ScaleData (void);

#endif /* INC_JOYSTICK_ADC_H_ */
