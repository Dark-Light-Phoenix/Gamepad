#include "Type-C.h"
#include "Cascads.h"
#include "Joystick ADC.h"
#include "stm32g4xx_hal.h"
#include "usb_device.h"

extern uint8_t Left, Up, Right, Down;
extern uint8_t Circle, Triangle, Chrest, Square;
extern uint8_t L1, L2;
extern uint8_t R1, R2;

const uint8_t HID_Gamepad_ReportDescriptor[] =
{
    0x05, 0x01,   // Usage Page (Generic Desktop)
    0x09, 0x05,   // Usage (Gamepad)
    0xA1, 0x01,   // Collection (Application)

    0x05, 0x09,   // Usage Page (Button)
    0x19, 0x01,   // Usage Minimum (Button 1)
    0x29, 0x0B,   // Usage Maximum (Button 11)
    0x15, 0x00,   // Logical Minimum (0)
    0x25, 0x01,   // Logical Maximum (1)
    0x95, 0x0B,   // Report Count (11 кнопок)
    0x75, 0x01,   // Report Size (1 біт)
    0x81, 0x02,   // Input (Data, Variable, Absolute)

    0x95, 0x05,   // Report Count (5 резервних бітів)
    0x75, 0x01,   // Report Size (1 біт)
    0x81, 0x03,   // Input (Constant) - Резерв

    0x05, 0x01,   // Usage Page (Generic Desktop)
    0x09, 0x30,   // Usage (X1)
    0x09, 0x31,   // Usage (Y1)
    0x09, 0x32,   // Usage (X2)
    0x09, 0x35,   // Usage (Y2)
    0x15, 0x81,   // Logical Minimum (-127)
    0x25, 0x7F,   // Logical Maximum (127)
    0x75, 0x08,   // Report Size (8 біт)
    0x95, 0x04,   // Report Count (4 осі)
    0x81, 0x02,   // Input (Data, Variable, Absolute)

    0xC0          // End Collection
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
    gamepad_report.x1 = ReadJoystickAxis (ADC_CHANNEL_1, hadc1);
    gamepad_report.y1 = ReadJoystickAxis (ADC_CHANNEL_3, hadc1);
    gamepad_report.x2 = ReadJoystickAxis (ADC_CHANNEL_2, hadc2);
    gamepad_report.y2 = ReadJoystickAxis (ADC_CHANNEL_3, hadc2);
}

void SendGamepadReport (void)
{
    UpdateButton();
    UpdateJoystick();
    USBD_HID_SendReport (&hUsbDeviceFS, &gamepad_report, sizeof (gamepad_report));
}
