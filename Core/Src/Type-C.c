#include "Type-C.h"
#include "stm32g4xx_it.h"

void USB_Connect (PCD_HandleTypeDef *hpcd_USB_FS)
{
	HAL_PCD_DevConnect(hpcd_USB_FS);
}

void USB_Disconnect (PCD_HandleTypeDef *hpcd_USB_FS)
{
	HAL_PCD_DevDisconnect(hpcd_USB_FS);
}

void USB_Set_Address (PCD_HandleTypeDef *hpcd_USB_FS, uint8_t address)
{
	HAL_PCD_SetAddress(hpcd_USB_FS, address);
}
