// Player.c
// Runs on LM4F120/TM4C123
// Brian Tsai, Benson Huang
// Last Modified: 05/04/2017
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
#include "SpriteMovements.h"
#include "Sprite.h"
#include "Player.h"
#include "Bomb.h"
#include "ExplosionDetection.h"




//variables;
Player * mainPlayer;
Sprite * currentSprite;

const unsigned short P1_DL[64];
const unsigned short P2_DL[64];

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Player_Init: Creates two players, sets their variables~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Player_Init(Player * newPlayer, int16_t startingX, int16_t startingY, int16_t idNumber){
				
		mainPlayer = newPlayer;
		mainPlayer->xPosition = startingX;
		mainPlayer->yPosition = startingY;
		mainPlayer->xPositionCenter = mainPlayer->xPosition + 4;
		mainPlayer->yPositionCenter = mainPlayer->yPosition - 4; 
		mainPlayer->playerId = idNumber;
		mainPlayer->currentSprite = mainPlayer->movement[0];
		mainPlayer->direction = standing;
		mainPlayer->xMultiplier = 1;
		mainPlayer->yMultiplier = 1;
		mainPlayer->stateOfLiving = alive;
		mainPlayer->playerPowerUp = NoPowerUp;
		mainPlayer->soundTimer = 0;
		Movement_Init(newPlayer);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Movement_Init changes Sprite direction~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Movement_Init(Player * player){
	ConfigureStandingSprite(player);
	ConfigureUpSprite(player);
	ConfigureDownSprite(player);
	ConfigureLeftSprite(player);
	ConfigureRightSprite(player);
	ConfigureDeadSprite(player);

}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Configure Standing Sprite~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ConfigureStandingSprite(Player * player){
	currentSprite = &mainPlayer->movement[0];
	Sprite_Init(currentSprite, player->xPosition, player->yPosition, 6, still);
	if(player->playerId == 1){
		InsertFrame(currentSprite, P1_DL);
	}
	if(player->playerId ==2){
		InsertFrame(currentSprite, P2_DL);
	}
	mainPlayer->currentSprite = mainPlayer->movement[0];
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Configure Up Sprite~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ConfigureUpSprite(Player * player){
	currentSprite = &mainPlayer->movement[1];
	Sprite_Init(currentSprite, player->xPosition, player->yPosition, 6, moving);
	if(player->playerId == 1){
		InsertFrame(currentSprite, P1_UL);
		InsertFrame(currentSprite, P1_UL);
		InsertFrame(currentSprite, P1_UL);
		InsertFrame(currentSprite, P1_UR);
		InsertFrame(currentSprite, P1_UR);
		InsertFrame(currentSprite, P1_UR);
	}
	if(player->playerId == 2){
		InsertFrame(currentSprite, P2_UL);
		InsertFrame(currentSprite, P2_UL);
		InsertFrame(currentSprite, P2_UL);
		InsertFrame(currentSprite, P2_UR);
		InsertFrame(currentSprite, P2_UR);
		InsertFrame(currentSprite, P2_UR);
	}	
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Configure Down Sprite~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ConfigureDownSprite(Player * player){
	currentSprite = &mainPlayer->movement[2];
	Sprite_Init(currentSprite, player->xPosition, player->yPosition, 6, moving);
	if(player->playerId == 1){
		InsertFrame(currentSprite, P1_DL);
		InsertFrame(currentSprite, P1_DL);
		InsertFrame(currentSprite, P1_DL);
		InsertFrame(currentSprite, P1_DR);
		InsertFrame(currentSprite, P1_DR);
		InsertFrame(currentSprite, P1_DR);
	}
		if(player->playerId == 2){
		InsertFrame(currentSprite, P2_DL);
		InsertFrame(currentSprite, P2_DL);
		InsertFrame(currentSprite, P2_DL);
		InsertFrame(currentSprite, P2_DR);
		InsertFrame(currentSprite, P2_DR);
		InsertFrame(currentSprite, P2_DR);
	}
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Configure Left Sprite~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ConfigureLeftSprite(Player * player){
	currentSprite = &mainPlayer->movement[3];
	Sprite_Init(currentSprite, player->xPosition, player->yPosition, 6, moving);
	if (player->playerId == 1){
		InsertFrame(currentSprite, P1_LL);
		InsertFrame(currentSprite, P1_LL);
		InsertFrame(currentSprite, P1_LL);
		InsertFrame(currentSprite, P1_LR);
		InsertFrame(currentSprite, P1_LR);
		InsertFrame(currentSprite, P1_LR);
	}
	if (player->playerId == 2){
		InsertFrame(currentSprite, P2_LL);
		InsertFrame(currentSprite, P2_LL);
		InsertFrame(currentSprite, P2_LL);
		InsertFrame(currentSprite, P2_LR);
		InsertFrame(currentSprite, P2_LR);
		InsertFrame(currentSprite, P2_LR);
	}
}
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Configure Right Sprite~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ConfigureRightSprite(Player * player){
	currentSprite = &mainPlayer->movement[4];
	Sprite_Init(currentSprite, player->xPosition, player->yPosition, 6, moving);
	if(player->playerId == 1){
		InsertFrame(currentSprite,P1_RL);
		InsertFrame(currentSprite,P1_RL);
		InsertFrame(currentSprite,P1_RL);
		InsertFrame(currentSprite,P1_RR);
		InsertFrame(currentSprite,P1_RR);
		InsertFrame(currentSprite,P1_RR);
	}
	if(player->playerId == 2){
		InsertFrame(currentSprite,P2_RL);
		InsertFrame(currentSprite,P2_RL);
		InsertFrame(currentSprite,P2_RL);
		InsertFrame(currentSprite,P2_RR);
		InsertFrame(currentSprite,P2_RR);
		InsertFrame(currentSprite,P2_RR);
	}	
}
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Configure Dead Sprite~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ConfigureDeadSprite(Player * player){
	currentSprite = &mainPlayer->movement[5];
	Sprite_Init(currentSprite, player->xPosition, player->yPosition, 6, moving);
	if (player->playerId == 1){
		InsertFrame(currentSprite,P1_Death1);
		InsertFrame(currentSprite,P1_Death2);
		InsertFrame(currentSprite,P1_Death3);
		InsertFrame(currentSprite,P1_Death4);
		InsertFrame(currentSprite,P1_Death5);
		InsertFrame(currentSprite,P1_Death6);
	}
	if (player->playerId == 2){
		InsertFrame(currentSprite,P2_Death1);
		InsertFrame(currentSprite,P2_Death2);
		InsertFrame(currentSprite,P2_Death3);
		InsertFrame(currentSprite,P2_Death4);
		InsertFrame(currentSprite,P2_Death5);
		InsertFrame(currentSprite,P2_Death6);
	}
}
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~update Player Perimeter~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void updatePlayerPerimeter(Player * player, Direction direction){
	switch(direction){
		case standing: break;
		case up: if(getVerticalVelocity(player) != 0){  
						 ST7735_FillRect(getXPosition(player), getYPosition(player) + 1, 8, 1 , ST7735_Color565(36, 108, 0)); break;}
						 else {break;}
		case down: if(getVerticalVelocity(player) != 0){
				       ST7735_FillRect(getXPosition(player), getYPosition(player) - 8, 8, 1, ST7735_Color565(36, 108, 0)); break;}
			       else {break;}
		case left: if(getHorizontalVelocity(player) != 0){
				       ST7735_FillRect(getXPosition(player) + 8, getYPosition(player) - 7, 1, 8, ST7735_Color565(36, 108, 0)); break;}
			         else {break;}
		case right: if(getHorizontalVelocity(player) != 0){
				        ST7735_FillRect(getXPosition(player) - 1, getYPosition(player) - 7, 1, 8, ST7735_Color565(36, 108, 0)); break;}
		            else {break;}
		case dead: break;
	}
}														

void setHorizontalVelocity(Player * player, int16_t horizontalVelocity){
	player->xVelocity = horizontalVelocity;
}

void setVerticalVelocity(Player * player, int16_t verticalVelocity){
	player->yVelocity = verticalVelocity;
}

void setHorizontalVelocityMultiplier(Player * player, int newXMultiplier){
	player->xMultiplier = newXMultiplier;
}

void setVerticalVelocityMultiplier(Player * player, int newYMultiplier){
	player->yMultiplier = newYMultiplier;
}

int16_t getHorizontalVelocity(Player * player){
	return player->xVelocity;
}

int16_t getVerticalVelocity(Player * player){
	return player->yVelocity;
}

int16_t getxMultiplier(Player * player){
	return player->xMultiplier;
}

int16_t getyMultiplier(Player * player){
	return player->yMultiplier;
}

void setXPosition(Player * player, int16_t newX){
	player->xPosition = newX;
	player->xPositionCenter = player->xPosition + 4;
}
int16_t getXPosition(Player * player){
	return player->xPosition;
}

void setYPosition(Player * player, int16_t newY){
	player->yPosition = newY;
	player->yPositionCenter = player->yPosition - 4;
}
int16_t getYPosition(Player * player){
	return player->yPosition;
}
	
void updateXPosition(Player * player){
	player->xPosition = player->xPosition + player->xVelocity;
	player->xPositionCenter = player->xPositionCenter + player->xVelocity;
}

void updateYPosition(Player * player){
	player->yPosition = player->yPosition + player->yVelocity;
	player->yPositionCenter = player->yPositionCenter + player->yVelocity;
}

void setDirection(Player * player, Direction newDirection) {
	player->direction = newDirection;
}

Direction getDirection(Player * player){
	return player->direction;
}

int16_t getXPositionCenter(Player * player){
	return player->xPositionCenter;
}

int16_t getYPositionCenter(Player * player){
	return player->yPositionCenter;
}

void setCurrentSprite(Player * player, int16_t newMovementIndex) {
	player->movementIndex = newMovementIndex;
	player->currentSprite = player->movement[player->movementIndex];	
}

void setPlayerPositionId(Player * player, int newPositionId){
	player->playerPositionId = newPositionId;
}

int16_t getPlayerPositionId(Player * player){
	return player->playerPositionId;
}

void Draw_Player(Player * currentPlayer) {
	ST7735_DrawBitmap(getXPosition(currentPlayer), getYPosition(currentPlayer), getFrame(&(currentPlayer->currentSprite)), 8, 8);
}

int16_t getPlayerId(Player * player){
	return player->playerId;
}

State checkPlayerStatus(Player * player){
	return player->stateOfLiving;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~determine State of Living, changing Player Status to notAlive~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void determineStateOfLiving(Player * currentPlayer1, Player * currentPlayer2, int16_t positionId){
	if (getExplosionRange(currentPlayer1, getExplosionXPosition(positionId), getExplosionYPosition(positionId)) == inRange){
		currentPlayer1->stateOfLiving = notAlive;
	}
	if (getExplosionRange(currentPlayer2, getExplosionXPosition(positionId), getExplosionYPosition(positionId)) == inRange){
		currentPlayer2->stateOfLiving = notAlive;
	}
}

void DrawPlayerDeath(Player * currentPlayer){
	ST7735_FillRect(getXPosition(currentPlayer), getYPosition(currentPlayer) - 7, 8, 8, ST7735_Color565(36, 108, 0));
}
	
	
