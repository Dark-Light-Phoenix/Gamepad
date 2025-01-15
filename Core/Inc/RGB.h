#ifndef INC_RGB_H_
#define INC_RGB_H_

#include "stm32g4xx_hal.h"

#define NUM_LEDS 8
#define LED_BITS 24
#define RES 280
#define PWM_HIGH 40
#define PWM_LOW 20

uint16_t pwmData [(LED_BITS * NUM_LEDS) + RES];

typedef enum {
	LIGHT_OFF = 1,
	LIGHT_ON
} LIGHT_ON_OFF;

typedef enum {
	GRADIENT = 1,
	BREATHING
} BACKLIGHT_MODES;

void Set_Color (uint8_t index, uint8_t green, uint8_t red, uint8_t blue);
void Prepare_Data (void);
void Send_data (void);
void HAL_TIM_PWM_PulseFinishedCallback (TIM_HandleTypeDef *htim);
void TIM7_IRQHandler (void);

#endif /* INC_RGB_H_ */
