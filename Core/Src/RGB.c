#include "RGB.h"
#include "stm32g4xx_it.h"
#include "stm32g4xx_hal.h"

extern DMA_HandleTypeDef hdma_tim17_ch1;
extern TIM_HandleTypeDef htim17;
extern uint8_t Left, Up, Right, Down;
extern uint8_t Circle, Triangle, Chrest, Square;

uint16_t pwmData [numLEDs * LEDbits];

int counter = 0;
int CPB = 0;

void setColor (uint8_t red, uint8_t green, uint8_t blue, int ledIndex)
{
	uint8_t ledData [3] = {green, red, blue};
	int bitIndex = ledIndex * LEDbits;

	for (int color = 0; color < 3; color++)
	{
		for (int bit = 0; bit < 8; bit++)
		{
			if (ledData [color] & (1 << (7 - bit)))
			{
				pwmData [bitIndex] = HIGH_DUTY_CICLE;
			}
			else
			{
				pwmData [bitIndex] = LOW_DUTY_CICLE;
			}
			bitIndex++;
		}
	}
	HAL_TIM_PWM_Start_DMA (&htim17, TIM_CHANNEL_1, (uint32_t*)pwmData, numLEDs * LEDbits);
	HAL_Delay (1);
	HAL_TIM_PWM_Stop_DMA (&htim17, TIM_CHANNEL_1);
}

void CounterSwitch (void)
{
	GPIO_PinState Backlight = HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_5);

	if (Backlight != 1)
	{
		counter += 1;
		if (counter > 3)
		{
			counter = 0;
		}
	}

	if (counter == 0)
	{
		clearLEDs();
	}

	if (counter == 1)
	{
		Blinking ();
	}

	/*if (counter == 1)
	{
		Blinking();
	}
	else if (counter == 2)
	{
		Gradient();
	}
	else if (counter == 3)
	{
		Wawe();
	}*/
}

void Blinking (void)
{
	for (int brightness_a = 0; brightness_a <= 255; brightness_a++)
	{
		for (int brightness_b = 0; brightness_b <= 255; brightness_b++)
		{
			for (int brightness_c = 0; brightness_c <= 255; brightness_c++)
			{
				for (int i = 0; i < numLEDs; i++)
				{
					setColor (brightness_a, brightness_b, brightness_c, i);
				}
				HAL_Delay(1000);
			}
		}

	}

	for (int brightness_a = 255; brightness_a >= 0; brightness_a--)
	{
		for (int brightness_b = 255; brightness_b >= 0; brightness_b--)
		{
			for (int brightness_c = 255; brightness_c >= 0; brightness_c--)
			{
				for (int i = 0; i < numLEDs; i++)
				{
					setColor (brightness_a, brightness_b, brightness_c, i);
				}
				HAL_Delay(1000);
			}
		}

	}
}

/*void Gradient (void)
{
    for (int j = 0; j < 256; j++)
    {
        for (int i = 0; i < numLEDs; i++)
        {
            uint8_t red = (i * j) % 256;
            uint8_t green = (i * 128) % 256;
            uint8_t blue = 255 - ((i * j) % 256);

            setColor(red, green, blue, i);
        }

        HAL_Delay(50);
    }
}

void Wawe (void)
{
	CheckButton ();

	int pressedButton = CPB;

    setColor(255, 0, 0, pressedButton);
    HAL_Delay(50);

    for (int offset = 1; offset < numLEDs; offset++)
    {
        uint8_t red = (255 - (offset * 30)) % 256;
        uint8_t green = (offset * 20) % 256;
        uint8_t blue = (offset * 50) % 256;
        if (pressedButton - offset >= 0)
        {
            setColor(red, green, blue, pressedButton - offset);
        }

        if (pressedButton + offset < numLEDs)
        {
            setColor(red, green, blue, pressedButton + offset);
        }

        HAL_Delay(50);
    }
}

void CheckButton (void)
{
	if (Left == GPIO_PIN_RESET)
	{
		CPB = 0;
	}
	if (Up == GPIO_PIN_RESET)
	{
		CPB = 1;
	}
	if (Right == GPIO_PIN_RESET)
	{
		CPB = 2;
	}
	if (Down == GPIO_PIN_RESET)
	{
		CPB = 3;
	}
	if (Circle == GPIO_PIN_RESET)
	{
		CPB = 4;
	}
	if (Triangle == GPIO_PIN_RESET)
	{
		CPB = 5;
	}
	if (Chrest == GPIO_PIN_RESET)
	{
		CPB = 6;
	}
	if (Square == GPIO_PIN_RESET)
	{
		CPB = 7;
	}
}*/

void clearLEDs (void)
{
    for (int i = 0; i < numLEDs; i++)
    {
        setColor(0, 0, 0, i);
    }
    HAL_TIM_PWM_Start_DMA(&htim17, TIM_CHANNEL_1, (uint32_t*)pwmData, numLEDs * LEDbits);
    HAL_Delay (1);
    HAL_TIM_PWM_Stop_DMA (&htim17, TIM_CHANNEL_1);
}
