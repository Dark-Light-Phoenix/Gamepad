#ifndef INC_RGB_H_
#define INC_RGB_H_

void setColor (uint8_t red, uint8_t green, uint8_t blue, int ledIndex);
void CounterSwitch (uint8_t *counter);
void Blinking (uint8_t *counter);
void Gradient (uint8_t *counter);
void Wawe (uint8_t *counter);
void CheckButton (void);
void SwitchMode (uint8_t *counter);

#endif /* INC_RGB_H_ */
