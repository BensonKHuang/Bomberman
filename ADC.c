// ADC.c
// Runs on LM4F120/TM4C123
// Brian Tsai, Benson Huang
// Last Modified: 05/04/2017

#include <stdint.h>
#include "tm4c123gh6pm.h"

// ADC initialization function 
// Input: none
// Output: none
void ADC_Init(void){ 
	volatile int delay;
	SYSCTL_RCGCGPIO_R |= 0x10;      // Turn on Port E clock 
  delay = 100;
  GPIO_PORTE_DIR_R &= ~0x04;      // Make PE4 input
  GPIO_PORTE_AFSEL_R |= 0x04;     // Enable alternate function on PE2
  GPIO_PORTE_DEN_R &= ~0x04;      // Disable digital I/O on PE2
  GPIO_PORTE_AMSEL_R |= 0x04;     // enable analog selection on PE2
  SYSCTL_RCGCADC_R |= 0x01;       // Turn on ADC0 clock 
  delay = SYSCTL_RCGCADC_R;       // Wait for clock to set
  delay = SYSCTL_RCGCADC_R;       
  delay = SYSCTL_RCGCADC_R;       
  delay = SYSCTL_RCGCADC_R;
  ADC0_PC_R = 0x01;               // Configure for 125K sps
  ADC0_SSPRI_R = 0x0123;          // Seq 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;        // Disable sample sequencer 3 to set settings
  ADC0_EMUX_R &= ~0xF000;         // Set Seq 3 to software trigger
  ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+1;  // Turn on Ain1 (PE2)
  ADC0_SSCTL3_R = 0x0006;         // No TS0 D0, yes IE0 END0
  ADC0_IM_R &= ~0x0008;           // Disable SS3 interrupts
  ADC0_ACTSS_R |= 0x0008;         // Enable sample sequencer 3


}

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t ADC_In(void){  
	uint32_t result;
	ADC0_PSSI_R = 0x0008;            // Start the ADC sampling
  while((ADC0_RIS_R&0x08)==0){};   // Wait for conversion done in RIS register
  result = ADC0_SSFIFO3_R&0xFFF;   // Load result 
  ADC0_ISC_R = 0x0008;             // acknowledge completion, clear RIS
  return result;	                 // Return result 
  
	
}


