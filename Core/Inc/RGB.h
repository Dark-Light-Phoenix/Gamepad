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
#define PWM_PERIOD 10
#define LONG_PRESS 1000
#define SHORT_PRESS 200
#define MAX_GRADIENT_COLOR 16

typedef enum {
	MODE_ON = 0,
	PULSE_MODE,
	GRADIENT_MODE,
	WAWE_EFFECT_MODE,
	CHASE_EFFECT,
	BREATHING_MODE,
	PROGRESSIVE_PATTERNS,

	MODE_OFF = 10
}LED_MODES;

void set_led_color (int ledIndex, uint8_t green, uint8_t red, uint8_t blue);
void send_single_bit (uint8_t bit);
void send_led_data (void);
void send_reset (void);
void leds_off (void);
void leds_on (void);
void pulse (void);
void gradient (void);
void wawe (void);
void chase (void);
void breathing (void);
void progressive (void);

#endif /* INC_RGB_H_ */
