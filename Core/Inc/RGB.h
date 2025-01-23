#ifndef INC_RGB_H_
#define INC_RGB_H_

#include "stm32g4xx_hal.h"

#define NUM_LEDS 8
#define LED_BITS 24
#define RES 280
#define PWM_HIGH 40
#define PWM_LOW 20
#define GRADIENT_STEPS 256
#define UPDATE_DELAY 50

void Set_Color (uint8_t index, uint8_t green, uint8_t red, uint8_t blue);
void Prepare_Data (void);
void Send_data (void);
void HAL_TIM_PWM_PulseFinishedCallback (TIM_HandleTypeDef *htim);
void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim);
void Breathing (void);
void Gradient (uint8_t position, uint8_t* green, uint8_t* red, uint8_t* blue);
void Update_Gradient (void);

typedef enum { // Structure for light on/off
	LIGHT_STATE_OFF,
	LIGHT_STATE_ON
} LightState;

typedef enum { // Structure for changing light modes
	LIGHT_MODE_BREATHING,
	LIGHT_MODE_GRADIENT,
	LIGHT_MODE_COUNT
} LightMode;

void HandleButtonPress (void);
void UpdateLightMode (void);
void TurnLightOff (void);
void TurnLightOn (void);

#endif /* INC_RGB_H_ */
