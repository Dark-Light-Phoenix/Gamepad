#include "Cascads.h"
#include "stm32g4xx_hal.h"
#include "usb_device.h"

extern uint8_t Left, Up, Right, Down;
extern uint8_t Circle, Triangle, Chrest, Square;
extern uint8_t L1, L2;
extern uint8_t R1, R2;

void Left_Cascade (uint8_t *Left, uint8_t *Up, uint8_t *Right, uint8_t *Down)
{
	*Left = HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_4);
	*Up = HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_5);
	*Right = HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_6);
	*Down = HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_7);
}

void Right_Cascade (uint8_t *Circle, uint8_t *Triangle, uint8_t *Chrest, uint8_t *Square)
{
	*Circle = HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_10);
	*Triangle = HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_9);
	*Chrest = HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_8);
	*Square = HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_0);
}

void Triggers (uint8_t *L1, uint8_t *L2, uint8_t *R1, uint8_t *R2)
{
	*L1 = HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_3);
	*L2 = HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_15);
	*R1 = HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_14);
	*R2 = HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_13);
}


