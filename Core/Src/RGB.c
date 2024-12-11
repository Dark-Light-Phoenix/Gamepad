#include "RGB.h"
#include "stm32g4xx_it.h"
#include "stm32g4xx_hal.h"

extern DMA_HandleTypeDef hdma_tim17_ch1;
extern TIM_HandleTypeDef htim17;

uint8_t pwmData [NUM_LEDS * LED_BITS];

int counter = 0;

void set_led_color (uint8_t green, uint8_t red, uint8_t blue, int ledIndex)
{
	uint8_t ledData [3] = {green, red, blue};
	int bitIndex = ledIndex * LED_BITS;

	for (int color = 0; color < 3; color++)
	{
		for (int bit = 0; bit < 8; bit++)
		{
			if (ledData [color] & (1 << (7 - bit)))
				{
					pwmData [bitIndex++] = T1H;
					pwmData [bitIndex++] = T1L;
				}
			else
				{
					pwmData [bitIndex++] = T1H;
					pwmData [bitIndex++] = T1L;
				}
		}
	}
	set_pwm();
}

void set_pwm (void)
{
	HAL_TIM_PWM_Start_DMA (&htim17, TIM_CHANNEL_1, (uint32_t*)pwmData, sizeof(pwmData));
	HAL_Delay (1);
}

void switch_counter (void)
{
	counter++;
	if (counter > 3)
		{
			counter = 0;
		}
	mode_switch(counter);
}

void mode_switch (uint8_t mode)
{
	switch(mode)
	{
		case MODE_OFF:
			leds_off();
			break;
		case PULSE_MODE:
			pulse();
			break;
		case GRADIENT_MODE:
			gradient();
			break;
		case WAWE_EFFECT_MODE:
			wawe();
			break;
	}
}

void leds_off (void)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        set_led_color(0, 0, 0, i);
    }
}

void pulse (void)
{
	for (int green_u = 0; green_u <= 256; green_u++)
	{
		for (int red_u = 0; red_u <= 256; red_u++)
		{
			for (int blue_u = 0; blue_u <= 256; blue_u++)
			{
				for (int i = 0; i < 8; i++)
				{
					set_led_color (green_u, red_u, blue_u, i);
				}
			}
		}
	}

	for (int green_d = 256; green_d >= 0; green_d--)
	{
		for (int red_d = 256; red_d >= 0; red_d--)
		{
			for (int blue_d = 256; blue_d >= 0; blue_d--)
			{
				for (int i = 0; i < 8; i++)
				{
					set_led_color (green_d, red_d, blue_d, i);
				}
			}
		}
	}

}

void gradient (void)
{

}

void wawe (void)
{

}
