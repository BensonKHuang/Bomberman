// Sound.c
// Runs on any computer
// Sound assets made from scratch
// Brian Tsai, Benson Huang 
// 05/04/2017
// Sound.c
// Runs on any computer
// Sound assets made from scratch
// Brian Tsai, Benson Huang 
// 05/04/2017
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Sound.h"
#include "DAC.h"
#include "Math.h"
#define PI 3.14159265358979323846
#include "Music.h"
#include "SysTick.h"

const uint32_t Bassoon[64] = {
  7,8,8,8,8,8,8,8,7,7,6,	
  7,8,10,14,15,12,7,2,0,0,2,	
  3,5,7,9,11,11,10,9,7,5,	
  3,2,2,3,5,7,9,10,9,7,	
  6,5,5,5,5,5,6,6,7,7,	
  7,8,8,9,9,8,8,8,8,8,7,7	
};


uint8_t *samplePt; // Holds the pointer of the sine table

uint8_t sampleIndex; // Holds the index of the sine table
uint8_t sampleList1[32]; // Sine table array
uint8_t sampleList2[32];
uint8_t sampleList3[32];
uint8_t sampleList4[32];

uint8_t BassoonIndex;
uint32_t level4; // Holds the calculated binary value
uint32_t level3;
uint32_t level2;
uint32_t level1;


void Sound_Init(void){
	NVIC_ST_CTRL_R &= ~0x1; // Disable Systick	
	NVIC_ST_RELOAD_R |= 0xFFFFFF; // Load the reload value with the period
	NVIC_ST_CURRENT_R |= 0; // Clear the current value
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF) | 0x40000000; // Set the priority level to level 2
	NVIC_ST_CTRL_R |= 0x7; // Enable interrupts, Systick enable and Core clock
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x20; // Activate Port F clock
	delay=100;		
	//GPIO_PORTF_LOCK_R = 0x4C4F434B;
  //GPIO_PORTF_CR_R |= 0x1;
	GPIO_PORTF_DIR_R |= 0x6; // Set PF
  GPIO_PORTF_AFSEL_R &= ~0x6;  
  //GPIO_PORTF_PDR_R |= 0x6;
  GPIO_PORTF_DEN_R |= 0x6; 
}

void Sound_SetPeriod(uint32_t period){
	TIMER3_TAILR_R = period - 1; // Set reload period to the passed value
	TIMER3_TAR_R = 0; // Clear the current value
}

void MusicArray_Init(){
	samplePt = sampleList1; //Initialize sampleList pointer
	uint32_t data = 0; 
	
	BassoonIndex = 0;
	
	for(int x = 0; x<16; x++){ // Initialize the sine table with the corresponding binary values
		level1 = (uint32_t) (round(((-1.65 * cos(11.25 * data * PI / 180) + 1.65) / 3.3) * 15)); 
		sampleList1[x] = level1;
		sampleList1[31-x] = level1;
		data ++;
		}
	for(int x = 0; x<16; x++){ // Initialize the sine table with the corresponding binary values
		level2 = (uint32_t) (round(((-0.825 * cos(11.25 * data * PI / 180) + 0.825) / 3.3) * 15)); 
		sampleList2[x] = level2;
		sampleList2[31-x] = level2;
		data ++;
		}
	for(int x = 0; x<16; x++){ // Initialize the sine table with the corresponding binary values
		level3 = (uint32_t) (round(((-0.4125 * cos(11.25 * data * PI / 180) + 0.4125) / 3.3) * 15)); 
		sampleList3[x] = level3;
		sampleList3[31-x] = level3;
		data ++;
		}
	for(int x = 0; x<16; x++){ // Initialize the sine table with the corresponding binary values
		level4 = (uint32_t) (round(((-0.20625 * cos(11.25 * data * PI / 180) + 0.20625) / 3.3) * 15)); 
		sampleList4[x] = level4;
		sampleList4[31-x] = level4;
		data ++;
		}
	
}

void Sound_Play(int songIndex){
	switch(songIndex){
		case 1: noteIndex = 0;
					  currentList = explosionList;
			      notePt = explosionList;
			      noteLengthIndex = 0;
			      noteLengthPt = explosionLengthList;
		        arraySize = 4;
			      break;
		case 2: noteIndex = 0;
		        currentList = deathList;
			      notePt = deathList;
			      noteLengthIndex = 0;
			      noteLengthPt = deathLengthList;
		        arraySize = 9;
		        break;
		case 3: noteIndex = 0;
			      currentList = menuList;
						notePt = menuList;
			      noteLengthIndex = 0;
			      noteLengthPt = menuLengthList;
		        arraySize = 125;
		        break;
		
		case 4: noteIndex = 0;
		        currentList = powerupList;   
			      notePt = powerupList;
			      noteLengthIndex = 0;
			      noteLengthPt = powerupLengthList;
						arraySize = 8;
						break;
		case 5: noteIndex = 0;
		        currentList = victoryList;
			      notePt = victoryList;
			      noteLengthIndex = 0;
			      noteLengthPt = victoryLengthList;
						arraySize = 7;
		        break;
						
		case 6: noteIndex = 0;
				currentList = battleList;
				notePt = battleList;
				noteLengthIndex = 0;
				noteLengthPt = battleLengthList;
				arraySize = 212;
				break;
			}		      
}


void Timer3A_Handler(void) {
		TIMER3_ICR_R |= 0x001; // Clear timeout flag
		sampleIndex = (sampleIndex + 1) % arraySize; // Go to the next sine value 
		if (Data >= 4000 && Data <= 5000){
			samplePt = &(sampleList1[sampleIndex]); // Get the new sine value
			DAC_Out(*samplePt); // Output the new sine value
		}
		else if (Data >= 3000 && Data < 4000){
			samplePt = &(sampleList2[sampleIndex]); // Get the new sine value
			DAC_Out(*samplePt); // Output the new sine value
		}
		else if (Data >= 2000 && Data < 3000){
			samplePt = &(sampleList3[sampleIndex]); // Get the new sine value
			DAC_Out(*samplePt); // Output the new sine value
		}
		else if (Data >= 1000 && Data < 2000){
			samplePt = &(sampleList4[sampleIndex]); // Get the new sine value
			DAC_Out(*samplePt); // Output the new sine value
		}
		
}



