// Piano.c
// This software configures the off-board piano keys
// Runs on LM4F120 or TM4C123
// Program written by: Brian Tsai & Benson Huang
// Date Created: 3/6/17 
// Last Modified: 3/6/17 
// Lab number: 10
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data
#include <stdint.h>
#include "tm4c123gh6pm.h"

// **************Piano_Init*********************
// Initialize piano key inputs, called once 
// Input: none 
// Output: none
void Piano_Init(void){
	
	volatile uint32_t delay;
	
	SYSCTL_RCGCGPIO_R |= 0x30; // Turn on Port F and E clock
  
	delay=100;									
  
	//GPIO_PORTF_LOCK_R = 0x4C4F434B;
	//GPIO_PORTE_LOCK_R = 0x4C4F434B;
	
  //GPIO_PORTF_CR_R |= 0x10;
	//GPIO_PORTE_CR_R |= 0xF;
  
	GPIO_PORTE_DIR_R &= ~0xF;	// Initialize PE0 - 3 as inputs  
	
	
  GPIO_PORTE_AFSEL_R &= ~0xF; // Disengage alternate function
  
	GPIO_PORTE_PDR_R |= 0xF; // Set inputs to positive logic
	 
  GPIO_PORTE_DEN_R |= 0xF; // Enable digital enable

}

// **************Piano_In*********************
// Input from piano key inputs 
// Input: none 
// Output: 0 to 7 depending on keys
// 0x01 is just Key0, 0x02 is just Key1, 0x04 is just Key2
uint32_t Piano_In(void){
 
//	buttonState = (GPIO_PORTE_DATA_R) & 0xF);
	return (GPIO_PORTE_DATA_R & 0xF); // Return the switches that were pressed
}
