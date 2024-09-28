#ifndef INC_RGB_H_
#define INC_RGB_H_

#include "stm32g4xx_hal.h"

#define numLEDs 8
#define LEDbits 24
#define HIGH_DUTY_CICLE 38
#define LOW_DUTY_CICLE 19

void setColor (uint8_t red, uint8_t green, uint8_t blue, int ledIndex);
void CounterSwitch (void);
void Blinking (void);
void Gradient (void);
void Wawe (void);
void CheckButton (void);
void clearLEDs (void);

#endif /* INC_RGB_H_ */
