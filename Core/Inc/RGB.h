#ifndef INC_RGB_H_
#define INC_RGB_H_

#include "stm32g4xx_hal.h"

#define NUM_LEDS 8
#define LED_BITS 24
#define RES 280
#define PWM_HIGH 40
#define PWM_LOW 20

void HandleButtonPress (GPIO_TypeDef *GPIO, uint16_t GPIO_Pin);
void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim);

typedef enum {
	STATIC = 0,
	SHORT,
	LONG,
	HOLDING
} ButtonState;

#endif /* INC_RGB_H_ */
