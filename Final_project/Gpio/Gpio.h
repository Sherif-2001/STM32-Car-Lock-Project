#ifndef GPIO_H
#define GPIO_H
#include "Std_Types.h"
#include "Rcc.h"

/*PortName*/
#define GPIO_A 'A'
#define GPIO_B 'B'
#define GPIO_C 'C'
#define GPIO_D 'D'

/*PinMode*/
#define GPIO_INPUT 0x00
#define GPIO_OUTPUT 0x01
#define GPIO_AF 0x02
#define GPIO_ANALOG 0x03

/*DefaultState*/
#define GPIO_PUSH_PULL 0x00
#define GPIO_OPEN_DRAIN 0x01

/* PUPDR Register States */
#define No_pull_up_pull_down 0x00
#define Pull_up 0x01
#define Pull_down 0x02
#define Reserved 0x03

/*Data*/
#define LOW 0x00
#define HIGH 0x01

// void Gpio_ConfigPin(uint8 PortName, uint8 PinNum, uint8 PinMode, uint8 DefaultState);
// void Gpio_WritePin(uint8 PortName, uint8 PinNum, uint8 Data);
uint8 Gpio_ReadPin(uint8 PortName, uint8 PinNum);

void GPIO_Init(void);
void GPIO_WritePinValue(unsigned char PinId, unsigned char PinData);
unsigned char GPIO_ReadPinState(unsigned char PinId);

#endif /* GPIO_H */
