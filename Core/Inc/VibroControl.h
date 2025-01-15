#ifndef INC_VIBROCONTROL_H_
#define INC_VIBROCONTROL_H_

#include "stm32g4xx_hal.h"

void HID_Report_Output (uint8_t *report);
void ControlVibration (uint8_t left_depth, uint8_t right_depth);

#endif /* INC_VIBROCONTROL_H_ */
