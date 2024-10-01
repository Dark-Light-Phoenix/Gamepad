#include "RGB.h"
#include "stm32g4xx_it.h"
#include "stm32g4xx_hal.h"

extern DMA_HandleTypeDef hdma_tim17_ch1;
extern TIM_HandleTypeDef htim17;
extern uint8_t Left, Up, Right, Down;
extern uint8_t Circle, Triangle, Chrest, Square;

uint16_t pwmData [(numLEDs * LEDbits) * 2];

int counter = 0;

void setColor (uint8_t green, uint8_t red, uint8_t blue, int ledIndex)
{
	uint8_t ledData [3] = {green, red, blue};
	int bitIndex = ledIndex * (LEDbits * 2);

	for (int color = 0; color < 3; color++)
	{
		for (int bit = 0; bit < 8; bit++)
		{
			if (ledData [color] & (1 << (7 - bit)))
			{
				pwmData [bitIndex] = T1H;
				pwmData [bitIndex + 1] = T1L;
			}
			else
			{
				pwmData [bitIndex] = T0H;
				pwmData [bitIndex + 1] = T0L;
			}
			bitIndex += 2;
		}
	}
	PWMPush();
}

void PWMPush (void)
{
	HAL_TIM_PWM_Start_DMA (&htim17, TIM_CHANNEL_1, (uint32_t*)pwmData, (numLEDs * LEDbits) * 2);
	HAL_Delay (10);
	HAL_TIM_PWM_Stop_DMA (&htim17, TIM_CHANNEL_1);
}

void CounterSwitch (void)
{
	counter++;
	if (counter > 3)
	{
		counter = 0;
	}
	ChangeMode (counter);
}

void ChangeMode (uint8_t mode)
{
	switch(mode)
	{
		case 0:
			clearLEDs();
			break;
		case 1:
			Blinking();
			break;
		case 2:
			Gradient();
			break;
		case 3:
			Wawe();
			break;
	}
}

void clearLEDs (void)
{
    for (int i = 0; i < numLEDs; i++)
    {
        setColor(0, 0, 0, i);
    }
}

void Blinking (void)
{
	for (int green_u = 0; green_u <= 256; green_u++)
	{
		for (int red_u = 0; red_u <= 256; red_u++)
		{
			for (int blue_u = 0; blue_u <= 256; blue_u++)
			{
				for (int i = 0; i < 8; i++)
				{
					setColor (green_u, red_u, blue_u, i);
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
					setColor (green_d, red_d, blue_d, i);
				}
			}
		}
	}

}

void Gradient (void)
{

}

void Wawe (void)
{

}
