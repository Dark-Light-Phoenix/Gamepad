#ifndef INC_RGB_H_
#define INC_RGB_H_

#include "stm32g4xx_hal.h"

#define numLEDs 8
#define LEDbits 24
#define T1H 34
#define T1L 14
#define T0H 14
#define T0L 45

void setColor (uint8_t green, uint8_t red, uint8_t blue, int ledIndex);
void PWMPush (void);
void CounterSwitch (void);
void ChangeMode (uint8_t mode);
void clearLEDs (void);
void Blinking (void);
void Gradient (void);
void Wawe (void);

#endif /* INC_RGB_H_ */
