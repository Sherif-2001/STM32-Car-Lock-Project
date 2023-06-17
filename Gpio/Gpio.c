
#include "Gpio.h"

#include "Gpio_Private.h"

// uint32 GPIO_Addresses[4] = { GPIOA_BASE_ADDR, GPIOB_BASE_ADDR,GPIOC_BASE_ADDR,GPIOD_BASE_ADDR };


// void Gpio_ConfigPin(uint8 PortName, uint8 PinNum, uint8 PinMode,
// 		uint8 DefaultState) {
// 	uint8 PortId = PortName - GPIO_A;
// 	GpioType * gpioReg =  GPIO_Addresses[PortId];

// 	uint8 OutputState = DefaultState & 0x1;
// 	uint8 InputState = DefaultState >> 1;

// 	gpioReg->GPIO_MODER &= ~(0x3 << (2 * PinNum));
// 	gpioReg->GPIO_MODER |= (PinMode << (2 * PinNum));

// 	gpioReg->GPIO_OTYPER  &= ~(0x01 << PinNum);
// 	gpioReg->GPIO_OTYPER |= (OutputState << PinNum);

// 	gpioReg->GPIO_PUPDR &= ~(0x3 << (2 * PinNum));
// 	gpioReg->GPIO_PUPDR |= (InputState << (2*PinNum));
// }


void GPIO_Init(void){
	Rcc_Init();
	Rcc_Enable(RCC_GPIOA);
	Rcc_Enable(RCC_GPIOD);
	Rcc_Enable(RCC_SYSCFG);

	uint32 i = 0;
	for (i = 5; i <= 15; i+=5) {

	 GpioType * gpioReg = GPIOD_BASE_ADDR;

	 gpioReg->GPIO_MODER &= ~(0x3 << (2 * i));
	 gpioReg->GPIO_MODER |= (GPIO_OUTPUT << (2 * i));

	 gpioReg->GPIO_OTYPER  &= ~(0x01 << i);
	 gpioReg->GPIO_OTYPER |= (GPIO_PUSH_PULL << i);
	  }

	for (i = 0; i <2; i++) {

	 GpioType * gpioReg = GPIOA_BASE_ADDR;

	 gpioReg->GPIO_MODER &= ~(0x3 << (2 * i));
	 gpioReg->GPIO_MODER |= (GPIO_INPUT << (2 * i));

         gpioReg->GPIO_PUPDR &= ~(0x3 << (2 * i));
 	 gpioReg->GPIO_PUPDR |= (Pull_up << (2*i));
   }

}


void GPIO_WritePinValue(unsigned char PinId, unsigned char PinData){
	GpioType * gpioReg =  GPIOD_BASE_ADDR;
	gpioReg->GPIO_ODR &= ~(0x01 << PinId);
	gpioReg->GPIO_ODR |= (PinData << PinId);
}


uint8 Gpio_ReadPin(uint8 PortName, uint8 PinNum) {
	uint8 data = 0;
	uint8 PortId = PortName - GPIO_A;
	GpioType * gpioReg =  GPIO_Addresses[PortId];
	data = (gpioReg->GPIO_IDR & (1 << PinNum)) >> PinNum;
	return data;
}










