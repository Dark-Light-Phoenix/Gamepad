#include "Type-C.h"
#include "Joystick ADC.h"
#include "stm32g4xx_hal.h"
#include "usb_device.h"
#include "usbd_hid.h"

extern uint8_t Left, Up, Right, Down;
extern uint8_t Circle, Triangle, Chrest, Square;
extern uint8_t L1, L2;
extern uint8_t R1, R2;
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern USBD_HandleTypeDef hUsbDeviceFS;
extern int8_t adc_x1, adc_x2;
extern int8_t adc_y1, adc_y2;

const uint8_t HID_ReportDescriptor[] =
{
    0x05, 0x01,
    0x09, 0x05,
    0xA1, 0x01,

    0x05, 0x09,
    0x19, 0x01,
    0x29, 0x0F,
    0x15, 0x00,
    0x25, 0x01,
    0x95, 0x0F,
    0x75, 0x01,
    0x81, 0x02,

    0x95, 0x05,
    0x75, 0x01,
    0x81, 0x03,

    0x05, 0x01,
    0x09, 0x30,
    0x09, 0x31,
    0x09, 0x32,
    0x09, 0x35,
    0x16, 0x00,   0xFF,
    0x26, 0x00,   0x01,
    0x75, 0x10,
    0x95, 0x04,
    0x81, 0x02,

	0x05, 0x0F,
	0x09, 0x21,
	0xA1, 0x02,

	0x09, 0x70,
	0x09, 0x71,
	0x15, 0x00,
	0x26, 0xFF, 0x00,
	0x75, 0x08,
	0x95, 0x01,
	0x91, 0x02,

	0xC0,
    0xC0
};

GamepadReport_TypeDef gamepad_report = {0};

void UpdateButton (void)
{
    gamepad_report.buttons = 0;

    if (Left)      gamepad_report.buttons |= (1 << 0);
    if (Up)        gamepad_report.buttons |= (1 << 1);
    if (Right)     gamepad_report.buttons |= (1 << 2);
    if (Down)      gamepad_report.buttons |= (1 << 3);
    if (Circle)    gamepad_report.buttons |= (1 << 4);
    if (Triangle)  gamepad_report.buttons |= (1 << 5);
    if (Chrest)    gamepad_report.buttons |= (1 << 6);
    if (Square)    gamepad_report.buttons |= (1 << 7);
}

void UpdateJoystick (void)
{
	ScaleData();
    gamepad_report.x1 = adc_x1;
    gamepad_report.y1 = adc_y1;
    gamepad_report.x2 = adc_x2;
    gamepad_report.y2 = adc_y2;
}

void HAL_ADC_ConvCpltCallback (ADC_HandleTypeDef* hadc)
{
	if (hadc == &hadc1 || hadc == &hadc2)
	{
		SendReport ();
	}
}

void SendReport (void)
{
    UpdateButton();
    UpdateJoystick();
    USBD_HID_SendReport (&hUsbDeviceFS, (uint8_t *) &gamepad_report, sizeof (gamepad_report));
}
