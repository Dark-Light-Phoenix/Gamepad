#include "ADC.h"
#include "HID.h"
#include "Buttons and Functions.h"
#include "stm32g4xx_hal.h"
#include "usb_device.h"
#include "usbd_customhid.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

GamepadReport_TypeDef gamepad_report = {0};

void SendReport (void)
{
	gamepad_report.report_id = 0x01;

    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&gamepad_report, sizeof(gamepad_report));
}
