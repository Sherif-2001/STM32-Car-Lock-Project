
#include "Bit_Operations.h"
#include "Gpio.h"
#include "Rcc.h"
#include "Std_Types.h"
#include "Exti.h"
#include "Gpt.h"

#define    vehicle_Led      5
#define    hazard_Led      10
#define    ambient_Led      15

    int current_state;
    int current_state2;
    int previous_state=1;
    int previous_state2=1;

int locked=1;
int closed=1;
int numberOfBlinks=0;
unsigned long int ambLightDuration=-1;
unsigned long int antiTheftTime=9999;
unsigned long int nextBlinkTime;
unsigned char  hazardLedState=0;
void blinks_init(int blinks);

int main() {

	Rcc_Init();
	Rcc_Enable(RCC_GPIOA);
	Rcc_Enable(RCC_GPIOD);
	Rcc_Enable(RCC_SYSCFG);
    GPIO_Init();
	GPT_Init();

	while (1) {

		current_state= Gpio_ReadPin(GPIO_A,0);
		current_state2= Gpio_ReadPin(GPIO_A,1);

	       	if ( (previous_state ==1) &&(current_state==0) && closed){ //if lock or unlock while door is closed
			  locked=!locked;

			  switch (locked){
			  case 0:
					GPT_StartTimer(10000);    // start timer for anti theft
			        GPIO_WritePinValue(vehicle_Led,1);  //vehicle lock light ON
			        GPIO_WritePinValue(ambient_Led,1);    // Ambient light ON
			        blinks_init(2);
			        ambLightDuration=2000;
			        break;
			  case 1:
					GPT_StartTimer(2000);      // start timer for blink
			        GPIO_WritePinValue(vehicle_Led,0);  //vehicle lock light OFF
			        GPIO_WritePinValue(ambient_Led,0);    // Ambient light OFF
			        blinks_init(4);

		            break;
			  default:
				  break;
	       	}


	}

			if(	((previous_state2 ==1) &&(current_state2==0))&& !locked){ // if opened or closed while car is unlocked			{
			  closed=!closed;

			  switch (closed){
			  case 0:
	             	GPIO_WritePinValue(ambient_Led,1);     // Ambient light ON
			        break;
			  case 1:
					GPT_StartTimer(10000);           // start timer for anti theft
			        GPIO_WritePinValue(vehicle_Led,0);  //vehicle lock light OFF
			        GPIO_WritePinValue(hazard_Led,0);  // Hazard led OFF
			        GPIO_WritePinValue(ambient_Led,1);    // Ambient light ON
			        ambLightDuration=1000;
			        break;
			  default:
			 	     break;
			}
			  numberOfBlinks=0;
	}


			if (!locked && closed && (GPT_GetElapsedTime()==antiTheftTime))  // Anti theft mode
			{
				locked=1;                 // Automatic lock for the door
				GPT_StartTimer(2000);      // start timer for the blink
		        GPIO_WritePinValue(vehicle_Led,0);  //vehicle lock light OFF
		        GPIO_WritePinValue(hazard_Led,0);  // Hazard led OFF
		        blinks_init(4);

			}

			if(!locked && closed && GPT_GetElapsedTime()==ambLightDuration)   // Time to turn off Ambient light
			{
		        GPIO_WritePinValue(ambient_Led,0);    // Ambient light OFF
		        ambLightDuration=-1;
			}

			if (numberOfBlinks>0 && (GPT_GetElapsedTime()==nextBlinkTime))    // Blinking
			{
                GPIO_WritePinValue(hazard_Led,!(hazardLedState));        // Toggle light
                hazardLedState=!(hazardLedState);      // Toggle hazard light state
     	        nextBlinkTime+=500;                   // update next blink time
				numberOfBlinks--;                     // update number of remaining blinks
		     }

		    previous_state=current_state;
		    previous_state2=current_state2;


}
	return 0;
}


void blinks_init(int blinks){

    GPIO_WritePinValue(hazard_Led,1);     // Hazard light ON
    nextBlinkTime=500;
    numberOfBlinks= blinks-1;
    hazardLedState=1;
}













