// ScoreBoard.c
// Runs on LM4F120/TM4C123
// Updates ScoreBoard and Score 
// Brian Tsai, Benson Huang
// Last Modified: 05/04/2017
#include <stdint.h>
#include <stdlib.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
#include "ScoreBoard.h"
#include "Timer0.h"
#include "ScoreBoardImages.h"
#include "Map.h"
#include "Player.h"
// struct
const unsigned short P1_Score[441];
const unsigned short P2_Score[441];

typedef enum {noTime, stillTime} TimeStatus;

struct Clock {
	int16_t  xPosition_Image;
	int16_t  yPosition_Image;
	
	int16_t  xPosition_secondsHundreds;
	int16_t  yPosition_secondsHundreds;
	
	int16_t  xPosition_secondsTens;
	int16_t  yPosition_secondsTens;
	
	int16_t  xPosition_secondsOnes;
	int16_t  yPosition_secondsOnes;
	
	int16_t  secondsHundreds;
	int16_t  secondsTens;
	int16_t  secondsOnes;
	unsigned short const *clockImage;
	TimeStatus hasTime;
	
};
typedef struct Clock Clock;

struct Mascot {
	int16_t xPosition;
	int16_t yPosition;
	int16_t score;
	unsigned short const *mascotImage;
};
typedef struct Mascot Mascot;



struct ScoreBoard {
	int16_t xPosition;
	int16_t yPosition;
	Clock *clock;
	Mascot *mascot1;
	Mascot *mascot2;
	int8_t score1;
	int8_t score2;
};
typedef struct ScoreBoard ScoreBoard;
	
//variables
ScoreBoard mainScoreBoard;
Clock mainClock;
Mascot mascotRed;
Mascot mascotBlue;


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~ScoreBoard_Init to set mascot, score, and clock~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ScoreBoard_Init(void){
	
	ST7735_FillRect(4, 4, 120, 32, ST7735_Color565(0,180,0));
	
	mainScoreBoard.clock = &mainClock;
	mainScoreBoard.mascot1 = &mascotRed;
	mainScoreBoard.mascot2 = &mascotBlue;
	
	mainScoreBoard.xPosition = 4;
	mainScoreBoard.yPosition = 4;
	
	mainClock.xPosition_Image = 8;
	mainClock.yPosition_Image = 30;
	
	mainClock.xPosition_secondsHundreds = mainClock.xPosition_Image + 20;
	mainClock.yPosition_secondsHundreds = mainClock.yPosition_Image - 15;
	
	mainClock.xPosition_secondsTens = mainClock.xPosition_secondsHundreds + 13;
	mainClock.yPosition_secondsTens = mainClock.yPosition_Image - 15;
	
	mainClock.xPosition_secondsOnes = mainClock.xPosition_secondsTens + 13;
	mainClock.yPosition_secondsOnes = mainClock.yPosition_Image - 15;
	
	Clock_Init();
	Timer0_Init(&Update_Clock, 80000000);
	Mascot_Init();
	ST7735_DrawBitmap(mainClock.xPosition_Image, mainClock.yPosition_Image, Clock_block, 17, 21);
	Draw_Clock();
	Draw_Mascot();
	Draw_Score();
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Clock_Init: Set to 300 seconds~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Clock_Init(void){
	 mainClock.secondsHundreds = 3;
	 mainClock.secondsTens = 0; 
	 mainClock.secondsOnes = 0; 
	 mainClock.clockImage = Clock_block;
	 mainClock.hasTime = stillTime;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Mascot_Init: Holds Scores and Image Sprites~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Mascot_Init(void){
	mascotRed.score = 0;
	mascotBlue.score = 0;
	mascotRed.xPosition = mainClock.xPosition_secondsTens + 15;
	mascotRed.yPosition = mainClock.yPosition_Image - 15;
	mascotBlue.xPosition = mascotRed.xPosition + 28;
	mascotBlue.yPosition = mascotRed.yPosition;
	mascotRed.mascotImage = P1_Score;
	mascotBlue.mascotImage = P2_Score;
}

void Score_Init(void){
	mainScoreBoard.score1 = 0;
	mainScoreBoard.score2 = 0;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Update_Clock: Ticks at One Second Interval, changes perimet at 100 seconds and blinks warning ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Update_Clock(void){
	static int flipColor = 0;
	if (mainClock.secondsHundreds == 1 && mainClock.secondsTens == 0 && mainClock.secondsOnes >= 0 && mainClock.secondsOnes <= 5){	
		flipColor ^= 1;
		FlashScreen(flipColor);
	}
	if (mainClock.secondsHundreds == 0 && mainClock.secondsTens >= 0 && mainClock.secondsOnes >= 0){
		GenerateNewPerimeter(&player1, &player2);
		GenerateNewPerimeter(&player1, &player2);
	}
	
	if (mainClock.secondsHundreds == 0 && mainClock.secondsTens == 0 && mainClock.secondsOnes == 0){
		Timer0A_Stop();
	}
	else if (mainClock.secondsOnes == 0 && mainClock.secondsTens > 0 ){
		mainClock.secondsTens --;
		mainClock.secondsOnes = 9;
	}
	else if (mainClock.secondsOnes == 0 && mainClock.secondsTens == 0){
		mainClock.secondsHundreds--;
		mainClock.secondsTens = 9;
		mainClock.secondsOnes = 9;
	}
	else { 
		mainClock.secondsOnes --;
	}
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~FlashScreen Function for Warning~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void FlashScreen(int flipColor){
	switch (flipColor){
		case 0: ST7735_FillRect(0, 0, 4, 160, ST7735_Color565(255,255,255));
			      ST7735_FillRect(0, 0, 128, 4, ST7735_Color565(255,255,255));
            ST7735_FillRect(0, 156, 128, 4, ST7735_Color565(255,255,255));
			      ST7735_FillRect(124, 0, 4, 160, ST7735_Color565(255,255,255));
		        break;
		case 1: ST7735_FillRect(0, 0, 4, 160, ST7735_Color565(0,0,0));
			      ST7735_FillRect(0, 0, 128, 4, ST7735_Color565(0,0,0));
            ST7735_FillRect(0, 156, 128, 4, ST7735_Color565(0,0,0));
			      ST7735_FillRect(124, 0, 4, 160, ST7735_Color565(0,0,0));
		        break;
	}
}
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~UpdateScore~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void UpdateScore(Player * player){
	switch(player->playerId){
		case 1: mainScoreBoard.score1 ++; break;
		case 2: mainScoreBoard.score2 ++; break;
	}
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ReturnScore for Logic~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int8_t ReturnScore(Player * player){
	if (player->playerId == 1){
		return mainScoreBoard.score1;
	}
	else{
		return mainScoreBoard.score2;
	}
}

void Draw_Clock(void){
	ST7735_DrawChar(mainClock.xPosition_secondsHundreds, mainClock.yPosition_secondsOnes, mainClock.secondsHundreds + 0x30, ST7735_Color565(255,255,255), ST7735_Color565(0,180,0), 2); 
	ST7735_DrawChar(mainClock.xPosition_secondsTens, mainClock.yPosition_secondsTens, mainClock.secondsTens + 0x30, ST7735_Color565(255,255,255), ST7735_Color565(0,180,0), 2); 
	ST7735_DrawChar(mainClock.xPosition_secondsOnes, mainClock.yPosition_secondsOnes, mainClock.secondsOnes + 0x30, ST7735_Color565(255,255,255), ST7735_Color565(0,180,0), 2); 
}

void Draw_Mascot(void){
	ST7735_DrawBitmap(mascotRed.xPosition + 13, mascotRed.yPosition + 20, mascotRed.mascotImage, 21, 21);
	ST7735_DrawBitmap(mascotBlue.xPosition + 13, mascotBlue.yPosition + 20, mascotBlue.mascotImage, 21, 21);
}

void Draw_Score(void){
	ST7735_DrawChar(mascotRed.xPosition + 26, mascotRed.yPosition, mainScoreBoard.score1 + 0x30, ST7735_Color565(216,11,10), ST7735_Color565(0,180,0), 2);
	ST7735_DrawChar(mascotBlue.xPosition + 26, mascotBlue.yPosition, mainScoreBoard.score2 + 0x30, ST7735_Color565(13,109,231), ST7735_Color565(0,180,0), 2);
}



	

