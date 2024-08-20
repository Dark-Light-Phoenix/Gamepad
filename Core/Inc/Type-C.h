#ifndef INC_TYPE_C_H_
#define INC_TYPE_C_H_

void USB_Connect (PCD_HandleTypeDef *hpcd_USB_FS);
void USB_Disconnect (PCD_HandleTypeDef *hpcd_USB_FS);
void USB_Set_Address (PCD_HandleTypeDef *hpcd_USB_FS, uint8_t address);

#endif /* INC_TYPE_C_H_ */
