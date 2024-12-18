#include "RGB.h"
#include "stm32g4xx_it.h"
#include "stm32g4xx_hal.h"
#include "stdlib.h"

extern DMA_HandleTypeDef hdma_tim17_ch1;
extern TIM_HandleTypeDef htim17;

uint8_t led_buffer[NUM_LEDS][3];
extern uint8_t current_gradient_index [NUM_LEDS];
extern uint8_t wave_start_led;
extern uint8_t wave_triggered;
extern uint8_t gradient_base_color [3];
static uint8_t step = 0;
static uint8_t brightness = 0;
static uint8_t direction = 1;
static uint8_t current_led = 0;

extern const uint8_t GRADIENT_BASES [MAX_GRADIENT_COLOR][3];

void set_led_color (int ledIndex, uint8_t green, uint8_t red, uint8_t blue)
{
	if (ledIndex < NUM_LEDS)
	{
		led_buffer[ledIndex][0] = green;
		led_buffer[ledIndex][1] = red;
		led_buffer[ledIndex][2] = blue;
	}
}

void send_single_bit (uint8_t bit)
{
	if (bit)
	{
		__HAL_TIM_SET_COMPARE (&htim17, TIM_CHANNEL_1, T1H);
	}
	else
	{
		__HAL_TIM_SET_COMPARE (&htim17, TIM_CHANNEL_1, T0H);
	}
}

void send_led_data (void)
{
	for (uint8_t i = 0; i < NUM_LEDS; i++)
	{
		for (uint8_t j = 7; j >= 0; j--)
		{
			send_single_bit ((led_buffer[i][0] > j) & 0x01);
		}
		for (uint8_t j = 7; j >= 0; j--)
		{
			send_single_bit ((led_buffer[i][1] > j) & 0x01);
		}
		for (uint8_t j = 7; j >= 0; j--)
		{
			send_single_bit ((led_buffer[i][2] > j) & 0x01);
		}
	}
	send_reset();
}

void send_reset (void)
{
	__HAL_TIM_SET_COMPARE (&htim17, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE (&htim17, TIM_CHANNEL_1, RES);

	HAL_TIM_PWM_Start (&htim17, TIM_CHANNEL_1);
	while (__HAL_TIM_GET_COUNTER (&htim17) < RES)
	{
		HAL_TIM_PWM_Stop (&htim17, TIM_CHANNEL_1);
	}
}

void leds_off (void)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        set_led_color(i, 0, 0, 0);
    }
    send_led_data();
}

void leds_on (void)
{
	for (int i = 0; i < NUM_LEDS; i++)
	{
		set_led_color(i, 255, 255, 255);
	}
	send_led_data();
}

void blinking (void)
{
	for (uint8_t i = 0; i < NUM_LEDS; i++)
	{
		set_led_color (i, brightness, brightness, brightness);
	}

	send_led_data();

	brightness += direction;
	if (brightness == 0 || direction == 255)
	{
		direction = -direction;
	}
}

void gradient (void)
{
	for (uint8_t i = 0; i < NUM_LEDS; i++)
	{
		set_led_color (i, (i * 32) % 256, (i * 16) % 256, (i * 8) % 256);
	}

	send_led_data();
}

void wawe (void)
{
	for (uint8_t i = 0; i < NUM_LEDS; i++)
	{
		int distance = abs (wave_start_led - i);

		if (distance <= step)
		{
			brightness = 255 - (distance * 30);
			if (brightness < 0)
			{
				brightness = 0;
			}
			set_led_color (i, (gradient_base_color [0] * brightness) / 255, (gradient_base_color [1] * brightness) / 255, (gradient_base_color [2] * brightness) / 255);
		}
		else
		{
			set_led_color (i, 0, 0, 0);
		}
	}

	send_led_data();

	step++;
	if ((wave_start_led + step >= NUM_LEDS) && (wave_start_led - step < 0))
	{
		step = 0;
		wave_triggered = 0;
	}
}

void chase (void)
{
	for (uint8_t i = 0; i < NUM_LEDS; i++)
	{
		set_led_color (i, 0, 0, 0);
	}

	set_led_color (current_led, gradient_base_color [0], gradient_base_color [1], gradient_base_color [2]);
	send_led_data();

	current_led = (current_led + 1) % NUM_LEDS;
}

void breathing (void)
{
	for (uint8_t i = 0; i < NUM_LEDS; i++)
	{
		set_led_color (i, (gradient_base_color [0] * brightness) / 255, (gradient_base_color [1] * brightness) / 255, (gradient_base_color [2] * brightness) / 255);
	}

	send_led_data();

	brightness += direction * 5;
	if (brightness >= 255)
	{
		direction = -1;
		brightness = 255;
	}
	else if (brightness <= 0)
	{
		direction = 1;
		brightness = 0;
	}
}

void progressive (void)
{
	for (uint8_t i = 0; i < NUM_LEDS; i++)
	{
		if (i <= step)
		{
			set_led_color (i, gradient_base_color [0], gradient_base_color [1], gradient_base_color [2]);
		}
		else
		{
			set_led_color (i, 0, 0, 0);
		}
	}

	send_led_data();

	step++;
	if (step >= NUM_LEDS)
	{
		step = 0;
	}
}
