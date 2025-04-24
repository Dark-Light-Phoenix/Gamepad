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
	ADC_Scale();
	USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&gamepad_report, 11);
}
