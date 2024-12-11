#ifndef INC_RGB_H_
#define INC_RGB_H_

#include "stm32g4xx_hal.h"

#define NUM_LEDS 8
#define LED_BITS 24
#define T0H 14
#define T1H 52
#define T0L 52
#define T1L 15
#define RES 13444

typedef enum {
	MODE_OFF = 0,
	PULSE_MODE,
	GRADIENT_MODE,
	WAWE_EFFECT_MODE
}LED_MODES;

void set_led_color (uint8_t green, uint8_t red, uint8_t blue, int ledIndex);
void set_pwm (void);
void switch_counter (void);
void mode_switch (uint8_t mode);
void leds_off (void);
void pulse (void);
void gradient (void);
void wawe (void);

#endif /* INC_RGB_H_ */
