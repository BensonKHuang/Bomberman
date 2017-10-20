// dac.c
// This software configures DAC output
// Runs on LM4F120 or TM4C123
// Program written by: Brian Tsai & Benson Huang
// Date Created: 3/6/17 
// Last Modified: 3/6/17 
// Lab number: 10
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

#include <stdint.h>
#include "tm4c123gh6pm.h"

// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data

//void DAC_Init(void);
//void DAC_Out(uint32_t data);


	
// **************DAC_Init*********************
// Initialize 4-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void){
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x2; // Turn on Port B clock
	delay=100;									
	GPIO_PORTB_LOCK_R = 0x4C4F434B;
  GPIO_PORTB_CR_R |= 0xF;
	GPIO_PORTB_DIR_R |= 0xF;	//Initialize PB0 - 3 as the outputs 
  GPIO_PORTB_AFSEL_R &=~0xF; // Disable alternate function
  GPIO_PORTB_PDR_R |= 0xF; // Set to positive logic
  GPIO_PORTB_DEN_R |= 0xF; // Enable digital enable  				
}

// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(uint32_t data){
	
	GPIO_PORTB_DATA_R = (data & 0xF); // Send the binary numbers in PB0 - 3 to the DAC to be converted 
	
}
