// SysTick.c
// Runs on LM4F120/TM4C123
// Brian Tsai, Benson Huang
// Last Modified: 05/04/2017

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "SysTick.h"
#include "ADC.h"

#define Sample 1000000
//#define Sample 
void SysTick_Init(uint32_t period){
	NVIC_ST_CTRL_R &= ~0x1; // Disable Systick
	NVIC_ST_RELOAD_R |= period-1; // Load the reload value with the period
	NVIC_ST_CURRENT_R |= 0; // Clear the current value
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF) | 0x40000000; // Set the priority level to level 2
	NVIC_ST_CTRL_R |= 0x7; // Enable interrupts, Systick enable and Core clock
	
}
void SysTick_Set(void){
	NVIC_ST_RELOAD_R = Sample - 1; // Set reload period to the passed value
	NVIC_ST_CURRENT_R = 0; // Clear the current value
}

/*void SysTick_Handler(void) {
	GPIO_PORTF_DATA_R ^= 0x4; //Toggle PF2 
	GPIO_PORTF_DATA_R ^= 0x4; // Toggle PF2
	ADCMail = ADC_In(); // Sample the data
	ADCStatus = 1; // Set the mailbox 
	GPIO_PORTF_DATA_R ^= 0x4; // Toggle PF2
} */

void SysTick_Handler(void){
	Data = ADC_In();
}





