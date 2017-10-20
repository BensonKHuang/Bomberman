// PlayerControls.c
// Runs on LM4F120/TM4C123
// Initialized Player Controls and Interrupt Handlers
// Brian Tsai, Benson Huang
// Last Modified: 05/04/2017
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
#include "Sprite.h"
#include "Player.h"
#include "Bomb.h"
#include "CollisionDetection.h"
#include "Inventory.h"

void EnableInterrupts(void);
void DisableInterrupts(void);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~Player1_Init (PortD) for Controls: Edge Triggered Interrupt~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Player1_Controls(void){
	volatile int delay = 0;   

	SYSCTL_RCGCGPIO_R|= 0x08;   // Set Port D clock
	delay = 100;               // Wait for clock to set
	GPIO_PORTD_LOCK_R = 0x4C4F434B;
  GPIO_PORTD_CR_R |= 0x8F;
	GPIO_PORTD_DIR_R &= (~0x8F);     // Set PD0-3 and PD7  to input 
	GPIO_PORTD_PDR_R |= 0x8F;
	GPIO_PORTD_AFSEL_R &= ~0x8F; // Disable analog function
	GPIO_PORTD_AMSEL_R &= ~0x8F;
	GPIO_PORTD_PCTL_R &= ~0xF000FFFF;
	GPIO_PORTD_DEN_R |= 0x8F;      // Set digital enable
	
	DisableInterrupts();
	GPIO_PORTD_IS_R &= ~0x8F;     // PortD is edge-sensitive
  GPIO_PORTD_IBE_R |= 0x8F;    // PortD are both edges
  GPIO_PORTD_ICR_R = 0x8F;      // (e) clear 0x8F
  GPIO_PORTD_IM_R |= 0x8F;      // (f) arm interrupt on 0x8F
	NVIC_PRI0_R = (NVIC_PRI0_R&0x1FFFFFFF)|0xD0000000; // (g) priority 5 (101)1
  NVIC_EN0_R = 0x00000008;      // (h) enable interrupt 3 in NVIC
  EnableInterrupts();           // (i) Enable global Interrupt flag (I)
	
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~Player2_Init (PortE and PortC) for Controls: Edge Triggered Interrupt~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Player2_Controls(void){
	volatile int delay = 0;   
	SYSCTL_RCGCGPIO_R|= 0x14;   		 // Set Port E and Port C clock
	delay = 100;              			 // Wait for clock to set
	GPIO_PORTE_LOCK_R = 0x4C4F434B;
	GPIO_PORTC_LOCK_R = 0x4C4F434B;
	
  GPIO_PORTE_CR_R |= 0x1B; 				 //Initialize PE0, 1, 3, 4, and PC 7 for Player 2 Controls (PE2 for ADC)
	GPIO_PORTC_CR_R |= 0x80;
	GPIO_PORTE_DIR_R &= (~0x1B);     // Set PE0-1, 3-4, and PC 7 to Input 
	GPIO_PORTC_DIR_R &= ~(0x80);
	GPIO_PORTE_PDR_R |= 0x1B;				 //set Pull Down Resistor for Positive Logic Switch
	GPIO_PORTC_PDR_R |= 0x80;
	GPIO_PORTE_AFSEL_R &= ~0x1B; 		 // Disable analog function
	GPIO_PORTC_AFSEL_R &= ~0x80;
	GPIO_PORTE_AMSEL_R &= ~0x1B;		 //Disable alternate function
	GPIO_PORTC_AMSEL_R &= ~0x80;
	GPIO_PORTE_PCTL_R &= ~0x000FF0FF;//Configure as GPIO
	GPIO_PORTC_PCTL_R &= ~0x000000F0;
	GPIO_PORTE_DEN_R |= 0x1B;      	 // Set digital enable
	GPIO_PORTC_DEN_R |= 0x80;
	
	DisableInterrupts();
	GPIO_PORTE_IS_R &= ~0x1B;     // Set PE0-1, 3-4, and PC 7 are edge-sensitive
  GPIO_PORTE_IBE_R |= 0x1B;    // Set PE0-1, 3-4, and PC 7 are both edges
  GPIO_PORTE_ICR_R = 0x1B;      // (e) clear PE0-1, 3-4, and PC 7
  GPIO_PORTE_IM_R |= 0x1B;      // (f) arm interrupt on PE0-1, 3-4, and PC 7 
	NVIC_PRI1_R = (NVIC_PRI1_R & 0xFFFFFF1F)|0x000000D0; // (g) priority 5 (101)1
  NVIC_EN0_R |= 0x00000010;      // (h) enable interrupt 4 in NVIC

	GPIO_PORTC_IS_R &= ~0x80;     // Set PE0-1, 3-4, and PC 7 are edge-sensitive
  GPIO_PORTC_IBE_R |= 0x80;    // Set PE0-1, 3-4, and PC 7 are both edges
  GPIO_PORTC_ICR_R = 0x80;      // (e) clear PE0-1, 3-4, and PC 7
  GPIO_PORTC_IM_R |= 0x80;      // (f) arm interrupt on PE0-1, 3-4, and PC 7 
	NVIC_PRI0_R = (NVIC_PRI0_R & 0xFF1FFFFF)|0x00D00000; // (g) priority 5 (101)1
  NVIC_EN0_R |= 0x00000004;      // (h) enable interrupt 2 in NVIC
  EnableInterrupts();           // (i) Enable global Interrupt flag (I)
	
}

void TurnOff_Player1(void){
	volatile int delay = 0;
	//SYSCTL_RCGCGPIOeewee_R &= ~(0x08);
	GPIO_PORTD_IM_R |= 0x8F;	
	NVIC_EN0_R &= ~(0x00000008);
	delay = 100;
	delay = 100;
}
void TurnOff_Player2(void){
	volatile int delay = 0;
	//SYSCTL_RCGCGPIO_R &= ~(0x14);
	GPIO_PORTC_IM_R &= ~(0x80);
	GPIO_PORTE_IM_R &= ~(0x1B);
	NVIC_EN0_R &= ~(0x00000010);
	NVIC_EN0_R &= ~(0x00000004);
	delay = 100;
	delay = 100;
};
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~Menu_Controls: Sets Busy wait for Player 1 in Menu~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Menu_Controls(void){
	volatile int delay = 0; 
	SYSCTL_RCGCGPIO_R|= 0x08;   // Set Port D clock
	delay = 100;               // Wait for clock to set
	GPIO_PORTD_LOCK_R = 0x4C4F434B;
  GPIO_PORTD_CR_R |= 0x8F;
	GPIO_PORTD_DIR_R &= (~0x8F);     // Set PD7, PD1 and PD 0 to input for menu
	GPIO_PORTD_PDR_R |= 0x8F;
	GPIO_PORTD_AFSEL_R &= ~0x8F; // Disable analog function
	GPIO_PORTD_AMSEL_R &= ~0x8F;
	GPIO_PORTD_PCTL_R &= ~0xF000FFFF;
	GPIO_PORTD_DEN_R |= 0x8F;      // Set digital enable
	};
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~GPIOPortD_Handler for Player 1 Controls ~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GPIOPortD_Handler(void){
	if(checkPlayerStatus(&player1) == alive){
		if ((GPIO_PORTD_DATA_R & 0xFF) == 0x01){
			GPIO_PORTD_ICR_R = 0x8F;
			setVerticalVelocity(&player1, -1);
			setDirection(&player1, up);
			setCurrentSprite(&player1, 1); //up
		}
		else if ((GPIO_PORTD_DATA_R & 0xFF)== 0x02){
			GPIO_PORTD_ICR_R = 0x8F;
			setVerticalVelocity(&player1, 1);
			setDirection(&player1, down);
			setCurrentSprite(&player1, 2); //down
		}
		else if ((GPIO_PORTD_DATA_R & 0xFF) == 0x04){
			GPIO_PORTD_ICR_R = 0x8F;
			setHorizontalVelocity(&player1, -1);
			setDirection(&player1, left);
			setCurrentSprite(&player1, 3); //left - 3
		}
		else if ((GPIO_PORTD_DATA_R & 0xFF) == 0x08){
			GPIO_PORTD_ICR_R = 0x8F;
			setHorizontalVelocity(&player1, 1);
			setDirection(&player1, right);
			setCurrentSprite(&player1, 4); //right - 4
		}
		else if ((GPIO_PORTD_DATA_R & 0xF0)== 0x80) {
			GPIO_PORTD_ICR_R = 0x8F;
			addBombToList(&inventory1, &player1, getWindowPlayerPositionId(&window1));
			UpdateBombGrid(getNewestBombFromList(&inventory1));
			//UpdateCountDownTime(&inventory1, &player1);
		}
		else{
			GPIO_PORTD_ICR_R = 0x8F;
			setHorizontalVelocity(&player1, 0);
			setVerticalVelocity(&player1, 0);
			setDirection(&player1, standing);
			setCurrentSprite(&player1, 0);
		}
	}
	else{
		GPIO_PORTD_ICR_R = 0x8F;
	}
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~GPIOPortE Handler for Player 2 Controls ~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GPIOPortE_Handler(void){
	if(checkPlayerStatus(&player2) == alive){
		if ((GPIO_PORTE_DATA_R & 0xFF) == 0x01){
			GPIO_PORTE_ICR_R = 0x1B;
			setVerticalVelocity(&player2, -1);
			setDirection(&player2, up);
			setCurrentSprite(&player2, 1); //up
		}
		else if ((GPIO_PORTE_DATA_R & 0xFF)== 0x02){
			GPIO_PORTE_ICR_R = 0x1B;
			setVerticalVelocity(&player2, 1);
			setDirection(&player2, down);
			setCurrentSprite(&player2, 2); //down
		}
		else if ((GPIO_PORTE_DATA_R & 0xFF) == 0x8){
			GPIO_PORTE_ICR_R = 0x1B;
			setHorizontalVelocity(&player2, -1);
			setDirection(&player2, left);
			setCurrentSprite(&player2, 3); //left - 3
		}
		else if ((GPIO_PORTE_DATA_R & 0xFF) == 0x10){
			GPIO_PORTE_ICR_R = 0x1B;
			setHorizontalVelocity(&player2, 1);
			setDirection(&player2, right);
			setCurrentSprite(&player2, 4); //right - 4
		}
		else{
			GPIO_PORTE_ICR_R = 0x1B;
			setHorizontalVelocity(&player2, 0);
			setVerticalVelocity(&player2, 0);
			setDirection(&player2, standing);
			setCurrentSprite(&player2, 0);
		}
	}
	else{
		GPIO_PORTE_ICR_R = 0x1B;
	}
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~GPIOPortC_Handler for Player 2 Controls (Bomb) ~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GPIOPortC_Handler(void){
	if (checkPlayerStatus(&player2) == alive){
		 if ((GPIO_PORTC_DATA_R & 0xF0) == 0x80) {
			addBombToList(&inventory2, &player2, getWindowPlayerPositionId(&window2));
			UpdateBombGrid(getNewestBombFromList(&inventory2));
		}
	}
	else {
		GPIO_PORTC_ICR_R = 0x80;
	}
}
