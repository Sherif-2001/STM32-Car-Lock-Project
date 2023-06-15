
#include "Gpio.h"

#include "Gpio_Private.h"

uint32 GPIO_Addresses[4] = { GPIOA_BASE_ADDR, GPIOB_BASE_ADDR,GPIOC_BASE_ADDR,GPIOD_BASE_ADDR };


void Gpio_ConfigPin(uint8 PortName, uint8 PinNum, uint8 PinMode,
		uint8 DefaultState) {
	uint8 PortId = PortName - GPIO_A;
	GpioType * gpioReg =  GPIO_Addresses[PortId];

	uint8 OutputState = DefaultState & 0x1;
	uint8 InputState = DefaultState >> 1;

	gpioReg->GPIO_MODER &= ~(0x3 << (2 * PinNum));
	gpioReg->GPIO_MODER |= (PinMode << (2 * PinNum));

	gpioReg->GPIO_OTYPER  &= ~(0x01 << PinNum);
	gpioReg->GPIO_OTYPER |= (OutputState << PinNum);

	gpioReg->GPIO_PUPDR &= ~(0x3 << (2 * PinNum));
	gpioReg->GPIO_PUPDR |= (InputState << (2*PinNum));
}


void GPIO_Init(void){
	Rcc_Init();
	Rcc_Enable(RCC_GPIOA);
	Rcc_Enable(RCC_GPIOD);
	Rcc_Enable(RCC_SYSCFG);

	Gpio_ConfigPin(GPIO_D, 5, GPIO_OUTPUT, GPIO_PUSH_PULL); //vehicle lock light
    Gpio_ConfigPin(GPIO_D, 10, GPIO_OUTPUT, GPIO_PUSH_PULL);  // Hazard led
    Gpio_ConfigPin(GPIO_D, 15, GPIO_OUTPUT, GPIO_PUSH_PULL);   // Ambient light led

	Gpio_ConfigPin(GPIO_A, 0, GPIO_INPUT, Pull_up); //door handle
	Gpio_ConfigPin(GPIO_A, 1, GPIO_INPUT, Pull_up);  //door button

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










