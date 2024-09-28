#include "Vibromotors.h"
#include "stm32g4xx_it.h"
#include "stm32g4xx_hal.h"
#include "usbd_hid.h"
#include "usb_device.h"

uint8_t left_motor;
uint8_t right_motor;
uint8_t report;

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim3;

void HID_Report_Output (uint8_t *report)
{
	left_motor = report [0];
	right_motor = report [1];

	ControlVibration (left_motor, right_motor);
}

void ControlVibration (uint8_t left_depth, uint8_t right_depth)
{
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, left_depth);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, right_depth);
}
