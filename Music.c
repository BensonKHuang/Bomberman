// Music.c
// This module contains the Timer1A ISR that plays music score
// Runs on LM4F120 or TM4C123
// Program written by: Brian Tsai & Benson Huang
// Date Created: 3/6/17 
// Last Modified: 3/6/17 
// Lab number: 10

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Music.h"


void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
void Sound_Play(int period);



uint8_t noteIndex; // Holds the index of noteList 
const uint32_t * notePt; // Holds the pointer of noteList
uint8_t noteLengthIndex; // Holds the index of noteLengthList 
const uint32_t * noteLengthPt; // Holds the pointer of noteLengthList
uint32_t arraySize;
const uint32_t *currentList;

void Timer3_Init(uint32_t period){
	volatile int delay;
	DisableInterrupts();
	SYSCTL_RCGCTIMER_R |= 0x08;   // 0) activate TIMER3
  //PeriodicTask1 = task;          // user function
	delay = 100;
  TIMER3_CTL_R = 0x00000000;    // 1) disable TIMER3A during setup
  TIMER3_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER3_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  //TIMER3_TBMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
	TIMER3_TAILR_R = 0xFFFFFE;   // 4) reload value
 // TIMER3_TBILR_R = 0xFFFE;
	TIMER3_TAPR_R = 0;            // 5) bus clock resolution
	TIMER3_TBPR_R = 0;  
  TIMER3_ICR_R = 0x00000001;    // 6) clear TIMER3A timeout flag
  TIMER3_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI8_R = (NVIC_PRI8_R&0x00FFFFFF)|0xA0000000; // 8) priority 4
	//NVIC_PRI9_R = (NVIC_PRI9_R&0xFFFFFF0F)|0x80;
	// interrupts enabled in the main program after all devices initialized
// vector number 37, interrupt number 21
  NVIC_EN1_R = 1<<3;           // 9) enable IRQ 25 in NVIC
	//NVIC_EN1_R = 1<<4;
	TIMER3_CTL_R = 0x00000001;    // 10) enable TIMER3
	EnableInterrupts();
}
/*
void Timer4_Init(uint32_t period){
	volatile int delay;
	DisableInterrupts();
	SYSCTL_RCGCTIMER_R |= 0x10;   // 0) activate TIMER3
  //PeriodicTask1 = task;          // user function
	delay = 100;
  TIMER4_CTL_R = 0x00000000;    // 1) disable TIMER3A during setup
  TIMER4_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER4_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  //TIMER3_TBMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
	TIMER4_TAILR_R = 0xFFFFFE;   // 4) reload value
 // TIMER3_TBILR_R = 0xFFFE;
	TIMER4_TAPR_R = 0;            // 5) bus clock resolution
	TIMER4_TBPR_R = 0;  
  TIMER4_ICR_R = 0x00000001;    // 6) clear TIMER3A timeout flag
  TIMER4_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI17_R = (NVIC_PRI17_R&0xFF00FFFF)|0x00800000; // 8) priority 4
	//NVIC_PRI9_R = (NVIC_PRI9_R&0xFFFFFF0F)|0x80;
	// interrupts enabled in the main program after all devices initialized
// vector number 37, interrupt number 21
  NVIC_EN2_R = 1<<6;           // 9) enable IRQ 25 in NVIC
	//NVIC_EN1_R = 1<<4;
	TIMER4_CTL_R = 0x00000001;    // 10) enable TIMER3
	EnableInterrupts();
}
*/
void Music_Init(void){
	noteIndex = 0;
	noteLengthIndex = 0;
}


void Timer3A_Start(void){
	TIMER3_CTL_R |= 0x1; // Start Timer1A
}

void Timer3A_Stop(void){
	TIMER3_CTL_R &= ~0x1; // Stop Timer1A
}
