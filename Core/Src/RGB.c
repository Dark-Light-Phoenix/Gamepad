#include "RGB.h"
#include "stm32g4xx_it.h"
#include "stm32g4xx_hal.h"

#define numLEDs 8
#define LEDbits 24

extern DMA_HandleTypeDef hdma_tim3_ch2;
extern TIM_HandleTypeDef htim3;
extern uint8_t Left, Up, Right, Down;
extern uint8_t Circle, Triangle, Chrest, Square;

uint8_t counter;

uint16_t pwmData [numLEDs * LEDbits];

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
}

void CounterSwitch (uint8_t *counter)
{
	GPIO_PinState buttonState = HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_5);

	if (buttonState == GPIO_PIN_SET)
	{
		(*counter)++;
		if (*counter > 3)
		{
			*counter = 0;
		}
		HAL_Delay(200);
	}
}

void Blinking (uint8_t *counter)
{
	for (int brightness = 0; brightness <= 255; brightness++)
	{
		for (int i = 0; i < nemLEDs; i++)
		{
			setColor (brightness, brightness, brightness, i);
		}
		HAL_Delay(10);
	}

	for (int brightness = 255; brightness >= 0; brightness--)
		{
			for (int i = 0; i < nemLEDs; i++)
			{
				setColor (brightness, brightness, brightness, i);
			}
			HAL_Delay(10);
		}
}

void Gradient(uint8_t *counter)
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

void Wawe (uint8_t *counter)
{
    int pressedButton = *counter;

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

    for (int i = 0; i < numLEDs; i++)
    {
        setColor(0, 0, 0, i);
    }
}

void CheckButton (void)
{
	if (Left == GPIO_PIN_SET)
	{
		Wawe(0);
	}
	if (Up == GPIO_PIN_SET)
	{
		Wawe(1);
	}
	if (Right == GPIO_PIN_SET)
	{
		Wawe(2);
	}
	if (Down == GPIO_PIN_SET)
	{
		Wawe(3);
	}
	if (Circle == GPIO_PIN_SET)
	{
		Wawe(4);
	}
	if (Triangle == GPIO_PIN_SET)
	{
		Wawe(5);
	}
	if (Chrest == GPIO_PIN_SET)
	{
		Wawe(6);
	}
	if (Square == GPIO_PIN_SET)
	{
		Wawe(7);
	}
}

void SwitchMode (uint8_t *counter)
{
	if (*counter == 1)
	{
		Blinking (counter);
	}
	else if (*counter == 2)
	{
		Gradient (counter);
	}
	else if (*counter == 3)
	{
		Wawe (counter);
	}
}
