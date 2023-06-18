#include "Gpio.h"
#include "Gpio_Private.h"

uint32 GPIO_Addresses[4] = {GPIOA_BASE_ADDR, GPIOB_BASE_ADDR, GPIOC_BASE_ADDR, GPIOD_BASE_ADDR};

void GPIO_Init(void)
{
	uint32 pinNum = 0;
	for (pinNum = 5; pinNum <= 15; pinNum += 5)
	{
		GpioType *gpioReg = (GpioType *)GPIOD_BASE_ADDR;
		gpioReg->GPIO_MODER &= ~(0x03 << (pinNum * 2));
		gpioReg->GPIO_MODER |= (GPIO_OUTPUT << (pinNum * 2));

		gpioReg->GPIO_OTYPER &= ~(0x01 << pinNum);
		gpioReg->GPIO_OTYPER |= (GPIO_PUSH_PULL << pinNum);
	}

	for (pinNum = 0; pinNum < 2; pinNum++)
	{
		GpioType *gpioReg = (GpioType *)GPIOA_BASE_ADDR;
		gpioReg->GPIO_MODER &= ~(0x03 << (pinNum * 2));
		gpioReg->GPIO_MODER |= (GPIO_INPUT << (pinNum * 2));

		gpioReg->GPIO_PUPDR &= ~(0x03 << (pinNum * 2));
		gpioReg->GPIO_PUPDR |= (Pull_up << (pinNum * 2));
	}
}

void GPIO_WritePinValue(unsigned char PinId, unsigned char PinData)
{
	GpioType *gpioReg = (GpioType *)GPIOD_BASE_ADDR;
	gpioReg->GPIO_ODR &= ~(0x01 << PinId);
	gpioReg->GPIO_ODR |= (PinData << PinId);
}

uint8 Gpio_ReadPin(uint8 PortName, uint8 PinNum)
{
	uint8 portId = PortName - GPIO_A;
	GpioType *gpioReg = (GpioType *)GPIO_Addresses[portId];
	return ((gpioReg->GPIO_IDR) >> PinNum) & 0x01;
}

unsigned char GPIO_ReadPinState(unsigned char PinId)
{
	char prev_state = 0;									   // initialize previous state to 0
	char curr_state = Gpio_ReadPin(GPIO_A, PinId);			   // read the current state of the pin
	char edge_detected = (prev_state == 1 && curr_state == 0); // check if falling edge detected
	prev_state = curr_state;								   // update previous state to current state
	if (edge_detected)
	{
		for (uint32 i = 0; i < 5000; i++)
			;
		curr_state = Gpio_ReadPin(GPIO_A, PinId);			  // read the state again after the delay
		edge_detected = (prev_state == 1 && curr_state == 0); // check for edge again
		prev_state = curr_state;							  // update previous state again
	}
	return edge_detected;
}