#ifndef INC_HID_H_
#define INC_HID_H_

#include "stm32g4xx_hal.h"

typedef struct __attribute__((packed))
{
	uint16_t buttons;
	uint8_t hat;
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;
} GamepadReport_TypeDef;

void SendReport (void);

#endif /* INC_HID_H_ */
