#ifndef INC_CASCADS_H_
#define INC_CASCADS_H_

#include "stm32g4xx_hal.h"

void Left_Cascade (uint8_t *Left, uint8_t *Up, uint8_t *Right, uint8_t *Down);
void Right_Cascade (uint8_t *Circle, uint8_t *Triangle, uint8_t *Chrest, uint8_t *Square);
void Triggers (uint8_t *L1, uint8_t *L2, uint8_t *R1, uint8_t *R2);

#endif /* INC_CASCADS_H_ */
