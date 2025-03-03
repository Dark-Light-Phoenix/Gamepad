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

//#define GRADIENT_STEPS 256
//
//typedef enum { // Structure for light on/off
//	LIGHT_STATE_OFF,
//	LIGHT_STATE_ON
//} LightState;
//
//typedef enum { // Structure for changing light modes
//	LIGHT_MODE_BREATHING,
//	LIGHT_MODE_GRADIENT,
//	LIGHT_MODE_COUNT
//} LightMode;
//
//volatile LightState currentLightState = LIGHT_STATE_OFF;
//volatile LightMode currentLightMode = LIGHT_MODE_BREATHING;

#define GPIOA_MASK (GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15)
#define GPIOB_MASK (GPIO_PIN_0 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7)
#define GPIO_INPUT_MASK_A (GPIO_PIN_3)
#define GPIO_INPUT_MASK_G (GPIO_PIN_10)

void Counter (void);

#endif /* INC_RGB_H_ */
