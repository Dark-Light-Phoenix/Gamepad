#ifndef INC_TYPE_C_H_
#define INC_TYPE_C_H_

#include "stm32g4xx_hal.h"

typedef struct
{
    uint16_t buttons;
    int8_t x1;
    int8_t y1;
    int8_t x2;
    int8_t y2;
} GamepadReport_TypeDef;

void UpdateButton (void);
void UpdateJoystick (void);
void SendReport (void);


#endif /* INC_TYPE_C_H_ */
