// Bomberman.c
// Runs on LM4F120/TM4C123
// Brian Tsai, Benson Huang
// Last Modified: 05/04/2017
 
// ******* Possible Hardware I/O connections*******************
// Player 1 Controls: (Up, Down, Left, Right, Bomb) = (PD0, PD1, PD2, PD3, PD7)
// Player 2 Controls: (Up, Down, Left, Right, Bomb) = (PE0, PE1, PE3, PE4, PC7)
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// ADC connected on PE2

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
#include "TExaS.h"
#include "ADC.h"
#include "DAC.h"
#include "Timer0.h"
#include "Timer1.h"
#include "Timer2.h"
#include "Map.h"
#include "ScoreBoard.h"
#include "SysTick.h"
#include "Sprite.h"
#include "Player.h"
#include "CollisionDetection.h"
#include "Struct.h"
#include "Bomb.h"
#include "Inventory.h"
#include "ExplosionDetection.h"
#include "PowerUps.h"
#include "Sound.h"
#include "Piano.h"
#include "DAC.h"
#include "Music.h"
#include "Menu.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds
void Player1_Controls(void);
void Player2_Controls(void);
void Menu_Controls(void);
void Disable_Player1(void);
void Disable_Player2(void);
int Player1_Input(void);
int Player2_Input(void);
void TurnOff_Player1(void);
void TurnOff_Player2(void);
//void UpdateGrid(void);

// *************************** Images ***************************




// *************************** Capture image dimensions out of BMP**********
typedef enum {noDeath, startDeath, endDeath, invincible, tieDeath, please} Death;

Death playerStatus1;
Death playerStatus2;
Window window1;
Window window2;
Player player1;
Player player2;
Inventory inventory1;
Inventory inventory2;
Supply supply;

//Menu
uint8_t pageNumber, level, Round;
uint32_t Data;

int main(void){
	TExaS_Init();  // set system clock to 80 MHz
	DAC_Init();		 // initialize noise system
	ADC_Init();		 // initialize volume controller
  Output_Init(); // initialize sitronix LED screen
	SysTick_Init(0x00FFFFFF);	//initialize SysTick for ADC
	Sound_Init();							//initialize Switch logic for sound  
	MusicArray_Init();				//initialize scores
	Music_Init();							//initialize Music
	Timer1_Init(0x00FFFFFF);	//initialize Timer1 and Timer3 for Music
	Timer3_Init(0x00FFFFFF);
	Delay100ms(20);
	pageNumber = 0;						//Page Number set up for menu
	while(1){
			/*~~~~~~~~~~~~~~~~~~~~~~Page 0: MENU SCREEN~~~~~~~~~~~~~~~~~~~~~~~~*/	
		if (pageNumber == 0){
			level = 3;
			Round = 1;
			ST7735_FillScreen(0x0000);
			ST7735_DrawBitmap(14, 63, SuperBomberman, 100, 47);		//LOGO
				
			ST7735_SetCursor(6,8);
			ST7735_OutString(">");
			ST7735_SetCursor(8,8);
			ST7735_OutString("BATTLE");
			ST7735_SetCursor(5,10);
			ST7735_OutString("INSTRUCTIONS");
			ST7735_SetCursor(5,14);
			ST7735_OutString("BRIAN TSAI &");
			ST7735_SetCursor(5,15);
			ST7735_OutString("BENSON HUANG");	
			Menu_Controls();											//Menu Controls use busy wait for menu inputs for Player 1
				
			//Menu Theme Music 
			//EnableInterrupts();
			Sound_Play(3);
			Timer1A_Start();
			Timer3A_Start();		
			while (pageNumber == 0){
				uint32_t input = (GPIO_PORTD_DATA_R & 0xFF);
				switch (input){
					case 0x01:
						ST7735_SetCursor(6,8);
						ST7735_OutString(">");
						ST7735_SetCursor(3,10);
						ST7735_OutString(" ");
						level = 3;
					break;
					case 0x02:
						ST7735_SetCursor(6,8);
						ST7735_OutString(" ");
						ST7735_SetCursor(3,10);
						ST7735_OutString(">");
						level = 2;
					break;			
					case 0x80:
						pageNumber = level;
							Score_Init();
					break;
				}
			}
		}
		Delay100ms(20);
			/*~~~~~~~~~~~~~~~~~~~~~~Page 1: GAME PLAY~~~~~~~~~~~~~~~~~~~~~~~~*/		
		if (pageNumber == 1){
			Random_Init(NVIC_ST_CURRENT_R);
			Timer1A_Stop();												// Stop Music once game starts
			Timer3A_Stop();
			ST7735_FillScreen(0x0000);            // set screen to black	
			//Map_Init(&supply);										// initialize Map generations
			//Init_ObjectGrid();										// initialize Object Grid
			Map_Init(&supply);										// Initialize Scoreboard
			NewPerimeter();
			Init_ObjectGrid();
			Player_Init(&player1, 12, 52, 1);			// initialize Player 1
			Player_Init(&player2, 108, 148, 2);		// initialize Player 2
			Inventory_Init(&inventory1);					// initialize Player 1 inventory (bomb)
			Inventory_Init(&inventory2);					// initialize Player 2 inventory (bomb)
			//ScoreBoard_Init();	
			Init_Window(&window1, &player1, 16);	// Initialize Window for Player 1 (perimeter checks)
			Init_Window(&window2, &player2, 208);	// Initialize Window for Player 2 (perimeter checks)
			//UpdatePlayerWindow(&window1, getDirection(&player1));
			//setPlayerPositionId(&player1, getWindowPlayerPositionId(&window1));
			//UpdatePlayerWindow(&window1, getDirection(&player2));
			//setPlayerPositionId(&player1, getWindowPlayerPositionId(&window2));
			Player1_Controls();										// Initialize PORTD for Player 1
			Player2_Controls();										// Initialize PORTC and PORTE for Player 2
			
			playerStatus1 = noDeath;							// set Player Status = noDeath for future Logic
			playerStatus2 = noDeath;
			ScoreBoard_Init();	
			Timer2_Init(4000000);	
			//Timer2A_Start();// initialize Timer2 for handler										
			while(pageNumber == 1){}							// Game loops in this while statement, until pageNumber changes
			ClearGrid();
			Delay100ms(20);
			}
			Delay100ms(20);
			/*~~~~~~~~~~~~~~~~~~~~~~Page 2: INSTRUCTIONS MANUAL~~~~~~~~~~~~~~~~~~~~~~~~*/	
		if (pageNumber == 2){
			ST7735_FillScreen(0x0000);
			//Instructions 
			ST7735_SetCursor(5,1);
			ST7735_OutString("INSTRUCTIONS");	
			
			ST7735_SetCursor(2,3);
			ST7735_OutString("WIN: Blow up other");
			ST7735_SetCursor(2,4);
			ST7735_OutString("     Player x3");
			ST7735_SetCursor(2,6);
			
			ST7735_OutString("MOVE: D-Pad");	
			ST7735_SetCursor(2,8);
			
			ST7735_OutString("BOMB: Action");		
			ST7735_SetCursor(2,10 );
			
			ST7735_OutString("POWERUP:  Last 30s");	
			ST7735_SetCursor(4,11);
			ST7735_OutString("Fire:   Range x2");	
			ST7735_SetCursor(4,12);
			ST7735_OutString("BombUp: Bombs x2");
			ST7735_SetCursor(4,13);
			ST7735_OutString("Skate:  Speed x2");		
			ST7735_SetCursor(6,15);
			ST7735_OutString("> RETURN");
			
			
			ST7735_DrawBitmap(82,66, P1_DL, 8, 8);
			ST7735_DrawBitmap(98,66, P2_DL, 8, 8);
			
			ST7735_DrawBitmap(86, 86, Bomb, 8, 8);
			ST7735_DrawBitmap(102, 86, Fire3, 8, 8);
			ST7735_DrawBitmap(110, 86, Fire3, 8, 8);
			ST7735_DrawBitmap(118, 86, Fire3, 8, 8);
			ST7735_DrawBitmap(110, 94, Fire3, 8, 8);
			ST7735_DrawBitmap(110, 78, Fire3, 8, 8);
			
			ST7735_DrawBitmap(12, 116, Power_Fire, 8, 8);
			ST7735_DrawBitmap(12, 126, Power_Bomb, 8, 8);
			ST7735_DrawBitmap(12, 136, Power_Skate, 8, 8);
			Menu_Controls();
			while (pageNumber == 2){
				if ((GPIO_PORTD_DATA_R & 0xFF) != 0){
					pageNumber = 0;
				}
			}
		}
		Delay100ms(20);
			/*~~~~~~~~~~~~~~~~~~~~~~Page 3: SCORE SCREEN BEFORE AND AFTER ROUNDS ~~~~~~~~~~~~~~~~~~~~~~~~*/
		if(pageNumber == 3){
			ST7735_FillScreen(0x05A0);
			ST7735_DrawString(22,20, "ROUND ", 0);
			ST7735_DrawChar(94, 20, Round + 0x30, 0 , ST7735_Color565(0,180,0), 2);   
			ST7735_DrawBitmap(22, 79, P1_Score, 21, 21);
			ST7735_DrawBitmap(85, 79, P2_Score, 21, 21);
			ST7735_DrawChar(28,100, ReturnScore(&player1) + 0x30,ST7735_Color565(13,109,231), ST7735_Color565(0,180,0), 2);
			ST7735_DrawChar(91,100 , ReturnScore(&player2) + 0x30, ST7735_Color565(216,11,10) , ST7735_Color565(0,180,0), 2);
			ST7735_DrawString(4,140, "> CONTINUE", 0);
			Round++;
			Delay100ms(20);	
			//DisableInterrupts()'	
			Menu_Controls();
			// Checks for Score here, and congratulates winner after 3 points
			if(ReturnScore(&player1) == 3 && ReturnScore(&player2) != 3){
				ST7735_DrawString(22,120, "P1 WINS!", 0);
				//Sound_Play(6);
				//Timer1A_Start();
				//Timer3A_Start();
				while(pageNumber == 3){
					int input = GPIO_PORTD_DATA_R & 0xFF;
					if (input != 0){
						pageNumber = 0;
					}
				}
			}		
			else if (ReturnScore(&player2) == 3 && ReturnScore(&player1) != 3){
				ST7735_DrawString(22,120, "P2 WINS!", 0);
				//Sound_Play(6);
				//Timer1A_Start();
				//Timer3A_Start();
				while(pageNumber == 3){
					int input = GPIO_PORTD_DATA_R & 0xFF;
					if (input != 0){
						pageNumber = 0;
					}
				}
			}
			else if (ReturnScore(&player2) == 3 && ReturnScore(&player1) == 3){
				ST7735_DrawString(22,120, " A TIE!", 0);
				//Sound_Play(6);
				//Timer1A_Start();
				//Timer3A_Start();
				while(pageNumber == 3){
					int input = GPIO_PORTD_DATA_R & 0xFF;
					if (input != 0){
						pageNumber = 0;
					}
				}
			}
			else {
				while(pageNumber == 3){
					int input = GPIO_PORTD_DATA_R & 0xFF;
					if (input != 0){
						pageNumber = 1;
					}
				}
			}
		}
		Delay100ms(20);
	}
}

// You can use this timer only if you learn how it works

void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 72724;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  } 
}

void Timer1A_Handler(void){
	TIMER1_ICR_R = TIMER_ICR_TATOCINT; // Clear timeout flag
	notePt = &currentList[noteIndex];
	TIMER1_TAILR_R = *noteLengthPt - 1;    // Set the new note length period in Timer1A // Get the new note 
	Sound_SetPeriod(*notePt); // Output the new note
	noteIndex = (noteIndex + 1) % arraySize; // Go to the next note 
	noteLengthIndex = (noteLengthIndex + 1) % arraySize; // Go to the next note length
	
}

void Timer2A_Handler(void){
  TIMER2_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER2A timeout
  //(*PeriodicTask0)();                // execute user task
	
//Player 1 Sprite Change
	if (player1.stateOfLiving == alive || playerStatus1 == invincible || (player1.stateOfLiving == notAlive && getAnimationStatus(&player1.currentSprite) != Done && playerStatus1 == startDeath)){
		Update_Sprite(&(player1.currentSprite));
		Draw_Player(&player1);
	}
//Player 2 Sprite Change
	if (player2.stateOfLiving == alive || playerStatus2 == invincible || (player2.stateOfLiving == notAlive && getAnimationStatus(&player2.currentSprite) != Done && playerStatus2 == startDeath)){
		Update_Sprite(&(player2.currentSprite));
		Draw_Player(&player2);
	}
	
//Player 1 Sprite Change for TIE
	if (player1.stateOfLiving == kamikaze && getAnimationStatus(&player1.currentSprite) != Done){
		Update_Sprite(&(player1.currentSprite));
		Draw_Player(&player1);
	}
	if (player1.stateOfLiving == kamikaze && getAnimationStatus(&player1.currentSprite) == Done){
		player1.stateOfLiving = finished;
	}
//Player 2 Sprite Change for TIE
	if (player2.stateOfLiving == kamikaze && getAnimationStatus(&player2.currentSprite) != Done){
		Update_Sprite(&(player2.currentSprite));
		Draw_Player(&player2);
	}
	if (player2.stateOfLiving == kamikaze && getAnimationStatus(&player2.currentSprite) == Done){
		player2.stateOfLiving = finished;
	}
	
//Player 1 State of Living Check	
	if ((player1.stateOfLiving == notAlive && getAnimationStatus(&player1.currentSprite) == Done && (playerStatus1 == startDeath)) || playerStatus1 == endDeath){
		if (playerStatus1 == startDeath){
			playerStatus1 = endDeath;
			DrawPlayerDeath(&player1);
		}
		if(player1.soundTimer == 61){
			Sound_Play(6);
			player1.soundTimer--;
		}
		else if (player1.soundTimer != 0){
			player1.soundTimer--;
		}
		else if (player1.soundTimer == 0 && playerStatus1 == endDeath){
			UpdateScore(&player2);
			Timer0A_Stop();
			Timer1A_Stop();
			Timer3A_Stop();
			pageNumber = 3;
			playerStatus1 = please;
			playerStatus2 = please;
			Timer2A_Stop();
			DisableInterrupts();			
		}
	}
	
//Player 2 State of Living Check
if ((player2.stateOfLiving == notAlive && getAnimationStatus(&player2.currentSprite) == Done && (playerStatus2 == startDeath)) || playerStatus2 == endDeath){
		if (playerStatus2 == startDeath){
			playerStatus2 = endDeath;
			DrawPlayerDeath(&player2);
		}
		if(player2.soundTimer == 61){
			Sound_Play(6);
			player2.soundTimer--;
		}
		else if (player2.soundTimer != 0){
			player2.soundTimer--;
		}
		else if (player2.soundTimer == 0 && playerStatus2 == endDeath){
			UpdateScore(&player1);
			Timer0A_Stop();
			Timer1A_Stop();
			Timer3A_Stop();
			pageNumber = 3;
			playerStatus1 = please;
			playerStatus2 = please;
			Timer2A_Stop(); 
			DisableInterrupts();			
		}
	}	
//Check for Tie Situation
	if (playerStatus1 == tieDeath && playerStatus2 == tieDeath){
	
		if(player1.stateOfLiving == finished){
			DrawPlayerDeath(&player1);
			player1.stateOfLiving = notAlive;
		}
		if(player2.stateOfLiving == finished){
			DrawPlayerDeath(&player2);
			player2.stateOfLiving = notAlive;
		}
		if(player2.soundTimer == 61){
			Sound_Play(6);
			player2.soundTimer--;
		}
		else if (player2.soundTimer != 0){
			player2.soundTimer--;
		}
		else{
			UpdateScore(&player1);
			UpdateScore(&player2);
			Timer0A_Stop();
			Timer1A_Stop();
			Timer3A_Stop();
			pageNumber = 3;
			playerStatus1 = please;
			playerStatus2 = please;
			Timer2A_Stop(); 
			DisableInterrupts();
		}	
	}	

//Player 1 Collision Detection 	
	for(int16_t x = 0; x < (getxMultiplier(&player1)); x++){
		UpdatePlayerId(&window1, getDirection(&player1));
		UpdatePlayerWindow(&window1, getDirection(&player1));
		setPlayerPositionId(&player1, getWindowPlayerPositionId(&window1));
		
		if ((Player_Bomb_Collision(&window1, &inventory1, &inventory2) == contact || Player_Block_Collision(&window1) == contact) && getDirection(&player1) == right) {
			setHorizontalVelocity(&player1, 0);
		}
		else if ((Player_Bomb_Collision(&window1, &inventory1, &inventory2) == contact || Player_Block_Collision(&window1) == contact) && getDirection(&player1) == left){
			setHorizontalVelocity(&player1, 0);
		}
		else if (( Player_Bomb_Collision(&window1, &inventory1, &inventory2) == contact || Player_Block_Collision(&window1) == contact )&& getDirection(&player1) == up){
			setVerticalVelocity(&player1, 0);
		}
		else if ((Player_Bomb_Collision(&window1, &inventory1, &inventory2) == contact || Player_Block_Collision(&window1) == contact) && getDirection(&player1) == down){
			setVerticalVelocity(&player1, 0);
		}
		else if (Player_Block_Collision(&window1) == clear){
			}
		else{
			setHorizontalVelocity(&player1, 0);
			setVerticalVelocity(&player1, 0);
		}
			updateXPosition(&player1);
			updateYPosition(&player1);
			if (player1.stateOfLiving == alive){
				Draw_Player(&player1);
			}
			updatePlayerPerimeter(&player1, player1.direction);
			Player_PowerUp_Collision(&window1, &supply);
	}

//Player 2 Collision Detection
	for(int16_t x = 0; x < (getxMultiplier(&player2)); x++){
		UpdatePlayerId(&window2, getDirection(&player2));
		UpdatePlayerWindow(&window2, getDirection(&player2));
		setPlayerPositionId(&player2, getWindowPlayerPositionId(&window2));
		
		if ((Player_Bomb_Collision(&window2, &inventory1, &inventory2) == contact || Player_Block_Collision(&window2) == contact) && getDirection(&player2) == right) {
			setHorizontalVelocity(&player2, 0);
		}
		else if ((Player_Bomb_Collision(&window2, &inventory1, &inventory2) == contact || Player_Block_Collision(&window2) == contact) && getDirection(&player2) == left){
			setHorizontalVelocity(&player2, 0);
		}
		else if (( Player_Bomb_Collision(&window2, &inventory1, &inventory2) == contact || Player_Block_Collision(&window2) == contact )&& getDirection(&player2) == up){
			setVerticalVelocity(&player2, 0);
		}
		else if ((Player_Bomb_Collision(&window2, &inventory1, &inventory2) == contact || Player_Block_Collision(&window2) == contact) && getDirection(&player2) == down){
			setVerticalVelocity(&player2, 0);
		}
		else if (Player_Block_Collision(&window2) == clear){
			}
		else{
			setHorizontalVelocity(&player2, 0);
			setVerticalVelocity(&player2, 0);
		}
			updateXPosition(&player2);
			updateYPosition(&player2);
			if (player2.stateOfLiving == alive){
				Draw_Player(&player2);
			}
			updatePlayerPerimeter(&player2, player2.direction);
			Player_PowerUp_Collision(&window2, &supply);
	}	
			
//Update Bomb and Explosion Deatails
		UpdatePlayerGrid(&player1);
		UpdatePlayerGrid(&player2);
		UpdatePowerUpTime(&supply, &player1, &player2);
		UpdateTickingBomb(&inventory1, &player1);
		UpdateTickingBomb(&inventory2, &player2);
		UpdateCountDownTime(&inventory1, &player1);
		UpdateCountDownTime(&inventory2, &player2);
		UpdateExplosion(&inventory1, &inventory2, &player1, &player2);
		//}
//Check if both die

	if (checkPlayerStatus(&player1) == notAlive && checkPlayerStatus(&player2) == notAlive && playerStatus1 == noDeath && playerStatus2 == noDeath){
		setHorizontalVelocity(&player1, 0);
		setVerticalVelocity(&player1, 0);
		setHorizontalVelocityMultiplier(&player1, 0);
		setVerticalVelocityMultiplier(&player1, 0);
		setCurrentSprite(&player1, 5);
		playerStatus1 = tieDeath;
		setHorizontalVelocity(&player2, 0);
		setVerticalVelocity(&player2, 0);
		setHorizontalVelocityMultiplier(&player2, 0);
		setVerticalVelocityMultiplier(&player2, 0);
		setCurrentSprite(&player2, 5);
		playerStatus2 = tieDeath;
		player2.soundTimer = 61;
		TurnOff_Player1();
		TurnOff_Player2();
		Sound_Play(2);
		Timer1A_Start();
		Timer3A_Start();
	}		
//Player 1 Death Check 
	if (checkPlayerStatus(&player1) == notAlive && playerStatus1 == noDeath){
		setHorizontalVelocity(&player1, 0);
		setVerticalVelocity(&player1, 0);
		setHorizontalVelocityMultiplier(&player1, 0);
		setVerticalVelocityMultiplier(&player1, 0);
		setCurrentSprite(&player1, 5);
		playerStatus1 = startDeath;
		playerStatus2 = invincible;
		player1.soundTimer = 61;
		TurnOff_Player1();
		TurnOff_Player2();
		Sound_Play(2);
		Timer1A_Start();
		Timer3A_Start();
	}
//Player 2 Death Check
	if (checkPlayerStatus(&player2) == notAlive  && playerStatus2 == noDeath){
		setHorizontalVelocity(&player2, 0);
		setVerticalVelocity(&player2, 0);
		setHorizontalVelocityMultiplier(&player2, 0);
		setVerticalVelocityMultiplier(&player2, 0);
		setCurrentSprite(&player2, 5);
		playerStatus2 = startDeath;
		playerStatus1 = invincible;
		player2.soundTimer = 61;
		TurnOff_Player1();
		TurnOff_Player2();
		Sound_Play(2);
		Timer1A_Start();
		Timer3A_Start();
	}
}




