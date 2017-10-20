// Bomb.c
// Runs on LM4F120/TM4C123
// Brian Tsai, Benson Huang
// Last Modified: 05/04/2017
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include <stdlib.h>
#include "ST7735.h"
#include "Random.h"
#include "BombStages.h"
#include "Map.h"
#include "Bomb.h"
#include "Sprite.h"
#include "ExplosionDetection.h"
#include "ExplosionStages.h"
#include "Inventory.h"

//Bombs bombGrid [15][15];
Animation * bombAnimation;
Sprite * bombSprite;
const unsigned short Fire3[64]; 
const unsigned short Bomb[64];

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Bomb_Init - Sets Variables for Bomb when placed~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Bomb_Init(Inventory * inventory, Player * player, Bombs * bomb, int16_t positionId, int16_t newBombId, int16_t newBombPlayer, int16_t numberofFrames){
	bomb->bombPlayer = newBombPlayer;
	UpdateBombGrid(getNewestBombFromList(inventory));
	bomb->xPosition = getBlockXPositionById(positionId);
	bomb->yPosition = getBlockYPositionById(positionId);
	bomb->xPositionCenter = bomb->xPosition + 4;
	bomb->yPositionCenter = bomb->yPosition - 4;
	bomb->positionId = positionId;
	bomb->animation.insertIndex = 0;
	bomb->animation.frameIndex = 0;
	bomb->animation.arraySize = numberofFrames;
	bomb->bombId = newBombId;
	
	bomb->detonationTime = 40;
	
	Sprite_Init(&(bomb->currentSprite), bomb->xPosition, bomb->yPosition, 6, moving);
	Sprite_Init(&(bomb->explosionCenter), bomb->xPosition, bomb->yPosition, 6, moving);
	//Sprite_Init(&(bomb->explosionUp), bomb->xPosition, bomb->yPosition, 6, moving);
	//Sprite_Init(&(bomb->explosionDown), bomb->xPosition, bomb->yPosition, 6, moving);
	//Sprite_Init(&(bomb->explosionLeft), bomb->xPosition, bomb->yPosition, 6, moving);
	//Sprite_Init(&(bomb->explosionRight), bomb->xPosition, bomb->yPosition, 6, moving);
	bomb->up = None;
	bomb->down = None;
	bomb->left = None;
	bomb->right = None;
	bomb->hasExploded = notExploded;
	bomb->firstTime = notSet;
	
	if (player->playerPowerUp == PowerFire){
		bomb->rangeOfExplosion = 2;
	}
	else {
		bomb->rangeOfExplosion = 1;
	}
	bombSprite = &(bomb->currentSprite);
	
	InsertFrame(bombSprite, Bomb);
	InsertFrame(bombSprite, Bomb2);
	InsertFrame(bombSprite, Bomb3);
	InsertFrame(bombSprite, Bomb3);
	InsertFrame(bombSprite, Bomb2);
	InsertFrame(bombSprite, Bomb);
	
	bombSprite = &(bomb->explosionCenter);
	InsertFrame(bombSprite, Fire1);
	InsertFrame(bombSprite, Fire2);
	InsertFrame(bombSprite, Fire3);
	InsertFrame(bombSprite, Fire3);
	InsertFrame(bombSprite, Fire2);
	InsertFrame(bombSprite, Fire1);
	
	UpdateBomb(player, bomb);
	
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Draw_BombOverlap: Layer of Graphics~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Draw_BombOverlap(Player * player, Bombs * bomb){
	int16_t row;
	int16_t col;
	const unsigned short* currentFrame = getFrame(&(bomb->currentSprite));
	for (row = 0; row < 8; row ++){
		for (col = 0; col < 8; col ++){
			if (bomb->snapshotFrame[row][col].isEmpty == blank){
				ST7735_DrawPixel(bomb->snapshotFrame[row][col].pixelXPosition, bomb->snapshotFrame[row][col].pixelYPosition, currentFrame[row * 8 + col]);
			}
		}
	}
	
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Draw_Bomb~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Draw_Bomb(Player * player, Bombs * bomb){
	int16_t row;
	int16_t col;
	const unsigned short* currentFrame = getFrame(&(bomb->currentSprite));
	for (row = 0; row < 8; row ++){
		for (col = 0; col < 8; col ++){
		 ST7735_DrawPixel(bomb->snapshotFrame[row][col].pixelXPosition, bomb->snapshotFrame[row][col].pixelYPosition, currentFrame[row * 8 + col]);
		}
	}
}
	
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Player_Snapshot~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Player_Snapshot(Player * player, Bombs * bomb, int16_t bombXPosition, int16_t bombYPosition, int16_t playerXPosition, int16_t playerYPosition){
	
	int16_t overlapLengthX = 8 - abs(playerXPosition - bombXPosition);
	int16_t overlapLengthY = 8 - abs(playerYPosition - bombYPosition);
	int16_t drawXPosition = playerXPosition - bombXPosition;
	int16_t drawYPosition = playerYPosition - bombYPosition;
	int16_t row = 0;
	int16_t col = 0;
	const unsigned short* playerFrame = getFrame(&(player->currentSprite));
	
		
		// If the overlap is centered
		if ( drawXPosition == 0 && drawYPosition == 0){
		playerFrame = getFrame(&(player->currentSprite));
			for (row = 0; row < 8; row ++){
				for (col = 0; col < 8; col ++){
					bomb->snapshotFrame[row][col].color = playerFrame[row * 8 + col];
					if (playerFrame[row * 8 + col] != 0x0364)
					{
						bomb->snapshotFrame[row][col].isEmpty = filled;
					}
					else {
						bomb->snapshotFrame[row][col].isEmpty = blank;
					}
				}
			}
		}
		// If the overlap is on the top side
		if ( drawYPosition < 0 && drawXPosition == 0) { 
		playerFrame = getFrame(&(player->currentSprite));
		for (row = 0; row < overlapLengthY; row ++){
			for (col = 0; col < 8; col++){
				
				bomb->snapshotFrame[row][col].color = playerFrame[(row + (8 - overlapLengthY)) * 8 + col];
				if (playerFrame[(row + (8 - overlapLengthY)) * 8 + col] != 0x0364)
				{
					bomb->snapshotFrame[row][col].isEmpty = filled;
				}
				else {
					bomb->snapshotFrame[row][col].isEmpty = blank;
				}
			}
		}
		for (row = overlapLengthY; row < 8; row ++){
			for (col = 0; col < 8; col ++){
				bomb->snapshotFrame[row][col].color = 0x0364;
				bomb->snapshotFrame[row][col].isEmpty = blank;
			}
		}
	}
		
		// If the overlap is on the bottom side
		if ( drawYPosition > 0 && drawXPosition == 0) { 
		playerFrame = getFrame(&(player->currentSprite));
		for (row = 8 - overlapLengthY; row < 8; row ++){
			for (col = 0; col < 8; col++){
				bomb->snapshotFrame[row][col].color = playerFrame[(row - (8 - overlapLengthY)) * 8 + col];
				if (playerFrame[(row - (8 - overlapLengthY)) * 8 + col] != 0x0364)
				{
					bomb->snapshotFrame[row][col].isEmpty = filled;
				}
				else {
					bomb->snapshotFrame[row][col].isEmpty = blank;
				}
			}
		}
		for (row = 0; row < 8 - overlapLengthY; row ++){
			for (col = 0; col < 8; col ++){
				bomb->snapshotFrame[row][col].color = 0x0364;
				bomb->snapshotFrame[row][col].isEmpty = blank;
			}
		}
	}
		
		// If the overlap is on the left side
	  if ( drawXPosition < 0 && drawYPosition == 0) { 
		playerFrame = getFrame(&(player->currentSprite));
		for (col = 0; col < overlapLengthX; col ++){
			for (row = 0; row < 8; row++){
				bomb->snapshotFrame[row][col].color = playerFrame[ col + (8 - overlapLengthY) + (8 * row)];
				if (playerFrame[ col + (8 - overlapLengthY) + (8 * row)] != 0x0364)
				{
					bomb->snapshotFrame[row][col].isEmpty = filled;
				}
				else {
					bomb->snapshotFrame[row][col].isEmpty = blank;
				}
			}
		}
		for (col = overlapLengthX; col < 8; col ++){
			for (row = 0; row < 8; row ++){
				bomb->snapshotFrame[row][col].color = 0x0364;
				bomb->snapshotFrame[row][col].isEmpty = blank;
			}
		}
	}
		
		// If the overlap is on the right side
		if ( drawXPosition > 0 && drawYPosition == 0) { 
		playerFrame = getFrame(&(player->currentSprite));
		for (col = 8 - overlapLengthX; col < 8 ; col ++){
			for (row = 0; row < 8; row++){
				bomb->snapshotFrame[row][col].color = playerFrame[ col - (8 - overlapLengthY) + (8 * row)];
				if (playerFrame[ col - (8 - overlapLengthY) + (8 * row)] != 0x0364)
				{
					bomb->snapshotFrame[row][col].isEmpty = filled;
				}
				else {
					bomb->snapshotFrame[row][col].isEmpty = blank;
				}
			}
		}
		for (col = 0; col < 8 - overlapLengthX; col ++){
			for (row = 0; row < 8; row ++){
				bomb->snapshotFrame[row][col].color = 0x0364;
				bomb->snapshotFrame[row][col].isEmpty = blank;
			}
		}
	}	
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Map Snapshot Grid~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void MapSnapshotGrid(Bombs * bomb){
	int16_t bombXPosition;
	int16_t bombYPosition;
	int16_t row = 0;
	int16_t col = 0;
	
	for (bombYPosition = bomb->yPosition; bombYPosition > bomb->yPosition - 8; bombYPosition--){
		for (bombXPosition = bomb->xPosition; bombXPosition < bomb->xPosition + 8; bombXPosition ++){
			bomb->snapshotFrame[row][col].pixelXPosition = bombXPosition;
			bomb->snapshotFrame[row][col].pixelYPosition = bombYPosition;
			col++;
		}
		col = 0;
		row++;
	}
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Update Bomb~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void UpdateBomb(Player * player, Bombs * bomb){
	if ( bomb->hasExploded == exploding){
		return;
	}
	Update_Sprite(&(bomb->currentSprite));
	Player_Snapshot(player, bomb, bomb->xPosition, bomb->yPosition, player->xPosition, player->yPosition);
	MapSnapshotGrid(bomb);
	Draw_BombOverlap(player, bomb);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Update Bomb Animation~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void UpdateBombAnimation(Player * player, Bombs * bomb){
	if ( bomb->hasExploded == exploding){
		return;
	}
	Update_Sprite(&(bomb->currentSprite));
	Draw_Bomb(player, bomb);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~get Bomb Position ID~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int16_t getBombPositionId(Bombs * bomb){
	return bomb->positionId;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~get Bomb ID~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int16_t getBombId(Bombs * bomb){
	return bomb->bombId;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~clear Bomb Range~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ClearBombRange(Bombs * bomb){
	bomb->up = None;
	bomb->down = None;
	bomb->left = None;
	bomb->right = None;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~get Bomb Range~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Range getBombRange(Player * player, Bombs * bomb){
	int16_t overlapX = abs(player->xPosition - bomb->xPosition);
	int16_t overlapY = abs(player->yPosition - bomb->yPosition);
	if ((overlapX >= 8 || overlapY >= 8) && bomb->firstTime == notSet){
		bomb->firstTime = set;
	}
	
	if (overlapX < 8 && overlapY < 8){
		return inRange;
	}
	else{
		return outOfRange;
	}
}

int16_t getBombXCenter(Bombs * bomb){
	return bomb->xPositionCenter;
}

int16_t getBombYCenter(Bombs * bomb){
	return bomb->yPositionCenter;
}

BombSet getBombSetStatus(Bombs * bomb){
	return bomb->firstTime;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Get Character Type~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
CharacterType getCharacterType(Bombs * bomb, Direction direction, int currentRange){
	CharacterType currentCharacterType;

	switch (currentRange){
		case 2: 
			switch (direction){
				case standing: currentCharacterType = None; break;
				case up: currentCharacterType = bomb->up2; break;
				case down: currentCharacterType = bomb->down2; break;
				case left: currentCharacterType = bomb->left2; break;
				case right: currentCharacterType = bomb->right2; break;
				case dead: currentCharacterType = None; break;
			} break;
		case 1: 
			switch (direction){
				case standing: currentCharacterType = None; break;
				case up: currentCharacterType = bomb->up; break;
				case down: currentCharacterType = bomb->down; break;
				case left: currentCharacterType = bomb->left; break;
				case right: currentCharacterType = bomb->right; break;
				case dead: currentCharacterType = None; break;
			} break;
	
		}
	return currentCharacterType;
}

