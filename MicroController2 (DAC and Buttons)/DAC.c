// dac.c
// This software configures DAC output
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 3/6/17 
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

#include <stdint.h>
#include "tm4c123gh6pm.h"
#define DACOut 0x3F;


// Initialize 6-bit DAC, called once 

void DAC_Init(void){
	volatile uint8_t delay=0;
	SYSCTL_RCGCGPIO_R |= 0x10;
	delay=42;
	GPIO_PORTE_AMSEL_R &= ~DACOut;	//disable analog function on PE1-0
  GPIO_PORTE_PCTL_R  &= ~DACOut; //enable regular GPIO
  GPIO_PORTE_DIR_R   |= DACOut;   // enable outputs for PE5-0
  GPIO_PORTE_AFSEL_R &= ~DACOut; // regular function on PE5-0
  GPIO_PORTE_DEN_R   |= DACOut;    // enable digital on PE5-0
}

// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(uint32_t data){
  GPIO_PORTE_DATA_R &= ~DACOut;
	GPIO_PORTE_DATA_R |= data;
}


