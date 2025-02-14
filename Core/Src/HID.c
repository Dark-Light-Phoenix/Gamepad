#include "ADC.h"
#include "HID.h"
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
volatile uint8_t buff1_update = 0;
volatile uint8_t buff2_update = 0;

char HID_ReportDescriptor[] = {

		// Mouse Report (ID 1)

		0x05, 0x01,					// Usage Page (Generic Desktop)
		0x09, 0x02,					// Usage (Mouse)
		0xA1, 0x01,					// Collection (Application)
		0x85, 0x01,					// Report ID (1)
		0x09, 0x01,					// Usage (Pointer)
		0xA1, 0x00,					// Collection (Physical)
		0x05, 0x09,					// Usage Page (Buttons)
		0x19, 0x01,					// Usage Minimum (1)
		0x29, 0x02,					// Usage Maximum (2)
		0x15, 0x00,					// Logical Minimum (0)
		0x25, 0x01,					// Logical Maximum (1)
		0x95, 0x02,					// Report Count (2)
		0x75, 0x01,					// Report Size (1)
		0x81, 0x05,					// Input (Data, Val, Abs)
		0x95, 0x06,					// Report Count (6)
		0x75, 0x01,					// Report Size (1)
		0x81, 0x05,					// Input (Const, Ary, Abs)
		0x05, 0x01,					// Usage Page (Generic Desktop)
		0x09, 0x30,					// Usage (X)
		0x09, 0x31,					// Usage (Y)
		0x09, 0x38,					// Usage (Wheel)
		0x15, 0x81,					// Logical Minimum (-127)
		0x25, 0x7F,					// Logical Maximum (+127)
		0x95, 0x02,					// Report Count (3)
		0x75, 0x08,					// Report Size (8)
		0x81, 0x06,					// Input (Data, Val, Rel)
		0xC0,						// End Collection (Physical)
		0xC0,						// End Collection (Application)

		// Consumer Control Report (ID 2) [Volume UP, Volume DOWN, Previous Track, Next Track, Play/Pause, Mute]

		0x05, 0x0C,					// Usage Page (Consumer Page)
		0x09, 0x01,					// Usage (Consumer Control)
		0xA1, 0x01,					// Collection (Application)
		0x85, 0x03,					// Report ID (2)
		0x09, 0xE9,					// Usage (Volume UP)
		0x09, 0xEA,					// Usage (Volume DOWN)
		0x15, 0x00,					// Logical Minimum (0)
		0x25, 0x01,					// Logical Maximum (1)
		0x95, 0x01,					// Report Count (2)
		0x75, 0x01,					// Report Size (1)
		0x81, 0x02,					// Input (Data, Val, Abs)
		0x09, 0xB5,					// Usage (Next Track)
		0x09, 0xB6,					// Usage (Previous Track)
		0x15, 0x00,					// Logical Minimum (0)
		0x25, 0x01,					// Logical Maximum (1)
		0x95, 0x01,					// Report Count (2)
		0x75, 0x01,					// Report Size (1)
		0x81, 0x02,					// Input (Data, Val, Abs)
		0x09, 0xCD,					// Usage (Play/Pause)
		0x15, 0x00,					// Logical Minimum (0)
		0x25, 0x01,					// Logical Maximum (1)
		0x95, 0x01,					// Report Count (1)
		0x75, 0x01,					// Report Size (1)
		0x81, 0x02,					// Input (Data, Val, Abs)
		0x09, 0xE2,					// Usage (Mute)
		0x15, 0x00,					// Logical Minimum (0)
		0x25, 0x01,					// Logical Maximum (1)
		0x95, 0x01,					// Report Count (1)
		0x75, 0x01,					// Report Size (1)
		0x81, 0x06,					// Input (Data, Val, Rel)
		0x95, 0x02,					// Report Count (2)
		0x75, 0x01,					// Report Size (1)
		0x81, 0x03,					// Input (Const, Val, Abs)
		0xC0,						// End of Collection

		// Keyboard Report (ID 3) [Esc, Backspace, Enter, Space]

		0x05, 0x01,					// Usage Page (Generic Desktop)
		0x09, 0x07,					// Usage (Keyboard)
		0xA1, 0x01,					// Collection (Application)
		0x85, 0x04,					// Report ID (3)
		0x09, 0x29,					// Usage (Esc)
		0x15, 0x00,					// Logical Minimum (0)
		0x25, 0x01,					// Logical Maximum (1)
		0x95, 0x01,					// Report Count (1)
		0x75, 0x01,					// Report Size (1)
		0x81, 0x02,					// Input (Data, Val, Abs)
		0x09, 0x2A,					// Usage (Backspace)
		0x15, 0x00,					// Logical Minimum (0)
		0x25, 0x01,					// Logical Maximum (1)
		0x95, 0x01,					// Report Count (1)
		0x75, 0x01,					// Report Size (1)
		0x81, 0x02,					// Input (Data, Val, Abs)
		0x09, 0x28,					// Usage (Enter)
		0x15, 0x00,					// Logical Minimum (0)
		0x25, 0x01,					// Logical Maximum (1)
		0x95, 0x01,					// Report Count (1)
		0x75, 0x01,					// Report Size (1)
		0x81, 0x02,					// Input (Data, Val, Abs)
		0x09, 0x2C,					// Usage (Space)
		0x15, 0x00,					// Logical Minimum (0)
		0x25, 0x01,					// Logical Maximum (1)
		0x95, 0x01,					// Report Count (1)
		0x75, 0x01,					// Report Size (1)
		0x81, 0x02,					// Input (Data, Val, Abs)
		0x95, 0x04,					// Report Count (4)
		0x75, 0x01,					// Report Size (1)
		0x81, 0x03,					// Input (Const, Val, Abs)
		0xC0,						// End of Collection (Application)

		// Gamepad Report (ID 4)

		0x05, 0x01,					// Usage Page (Generic Desktop)
		0x09, 0x05,					// Usage (Gamepad)
		0xA1, 0x01,					// Collection (Application)
		0x85, 0x04,					// Report ID (4)
		0x05, 0x09,					// Usage Page (Buttons)
		0x19, 0x01,					// Usage Minimum (1)
		0x29, 0x0E,					// Usage Maximum (15)
		0x15, 0x00,					// Logical Minimum (0)
		0x25, 0x01,					// Logical Maximum (1)
		0x95, 0x0E,					// Report Count (15)
		0x75, 0x01,					// Report Size (1)
		0x81, 0x02,					// Input (Data, Abs, Val)
		0x95, 0x01,					// Report Count (1)
		0x75, 0x01,					// Report Size (1)
		0x81, 0x03,					// Input (Const, Abs, Val)
		0x05, 0x01,					// Usage Page (Generic Desktop)
		0x09, 0x30,					// Usage (X)
		0x09, 0x31,					// Usage (Y)
		0x09, 0x32,					// Usage (RX)
		0x09, 0x35,					// Usage (RY)
		0x16, 0x00, 0xF8,			// Logical Minimum (-2048)
		0x26, 0xFF, 0x07,			// Logical Maximum (+2047)
		0x95, 0x04,					// Report Count (4)
		0x75, 0x10,					// Report Size (16)
		0x81, 0x02,					// Input (Data, Val, Abs)
		0xC0						// End Collection (Application)
};

GamepadReport_TypeDef gamepad_report = {0};
HID_Mode currentHIDMode = HID_MOD_MOUSE;

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
    if (R1)		   gamepad_report.buttons |= (1 << 8);
    if (R2)		   gamepad_report.buttons |= (1 << 9);
    if (L1)		   gamepad_report.buttons |= (1 << 10);
    if (L2)		   gamepad_report.buttons |= (1 << 11);
}

void UpdateJoystick (void)
{
    gamepad_report.x1 = adc_x1;
    gamepad_report.y1 = adc_y1;
    gamepad_report.x2 = adc_x2;
    gamepad_report.y2 = adc_y2;
}


void HAL_ADC_ConvCpltCallback (ADC_HandleTypeDef* hadc)
{
	if (hadc == &hadc1 || hadc == &hadc2)
	{
		ADC_Scale();
	}
}

void SendReport (void)
{
    UpdateButton();
    UpdateJoystick();
    USBD_HID_SendReport (&hUsbDeviceFS, (uint8_t *) &gamepad_report, sizeof (gamepad_report));
}
