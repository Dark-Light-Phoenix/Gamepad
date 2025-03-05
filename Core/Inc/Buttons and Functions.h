#ifndef INC_BUTTONS_AND_FUNCTIONS_H_
#define INC_BUTTONS_AND_FUNCTIONS_H_

#include "stm32g4xx_hal.h"

typedef enum {
	STATIC = 0,
	SHORT,
	LONG,
	HOLDING
} ButtonState;

#define GPIOA_MASK (GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15)
#define GPIOB_MASK (GPIO_PIN_0 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7)
#define GPIO_INPUT_MASK_A (GPIO_PIN_3)
#define GPIO_INPUT_MASK_G (GPIO_PIN_10)

void HandleButtonPress (GPIO_TypeDef *GPIO, uint16_t GPIO_Pin);
void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim);
void ButtonStatus (void);

#endif /* INC_BUTTONS_AND_FUNCTIONS_H_ */
