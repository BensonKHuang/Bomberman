// ExplosionDetection.c
// Runs on LM4F120/TM4C123
// Initializes Player and Object Grids, and determines Explosion detection on all objects
// Brian Tsai, Benson Huang
// Last Modified: 05/04/2017

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
#include "ExplosionDetection.h"
#include "Player.h"
#include "Bomb.h"
#include "Map.h"
#include "Sprite.h"
#include "Struct.h"
#include "PowerUps.h"
#include "Inventory.h"
#include <stdlib.h>
#include "Sound.h"
#include "Music.h"

Object objectGrid[15][15];
int16_t playerPrevious1;
int16_t playerPrevious2;	
void Timer1A_Start(void);
void Timer1A_Stop(void);

/*~~~~~~~~~Bomb_RayTrace: Every Explosion will set it's perimeters with Objects (Player, Blocks, Powerup)~~~~~~~~~*/
void Bomb_RayTrace(Inventory * inventory, Player * player, Bombs * bomb, int16_t bombPositionId){
	UpdatePlayerGrid(&player1);
	UpdatePlayerGrid(&player2);
	bomb->hasExploded = exploding;	
	UpdateBombStatusGrid(bomb, exploding);
	int currentExplosionRange;
	
	// Check if top has player
	for (currentExplosionRange = 1; currentExplosionRange < getPowerExplosionRange(bomb) + 1; currentExplosionRange ++){
		if (getCharacterType(bomb, up, currentExplosionRange) == None){
			if ((bombPositionId - 15 * currentExplosionRange)/15 > -1 && checkPlayerGrid(bombPositionId - 15 * currentExplosionRange) != -1){
				switch (currentExplosionRange){
					case 1: bomb->up = Player_Object; break;
					case 2: bomb->up2 = Player_Object; break;
				}
		}
	}
}
	
	// Check if bottom has player
	for (currentExplosionRange = 1; currentExplosionRange < getPowerExplosionRange(bomb) + 1; currentExplosionRange ++){
		if (getCharacterType(bomb, down, currentExplosionRange) == None){
			if ((bombPositionId + 15 * currentExplosionRange)/15 < 15 && checkPlayerGrid(bombPositionId + 15 * currentExplosionRange) != -1){
				switch (currentExplosionRange){
					case 1: bomb->down = Player_Object; break;
					case 2: bomb->down2 = Player_Object; break;
				}
			}
	}
}
	
	// Check if left has player
	for (currentExplosionRange = 1; currentExplosionRange < getPowerExplosionRange(bomb) + 1; currentExplosionRange ++){
		if (getCharacterType(bomb, left, currentExplosionRange) == None){
			if ((bombPositionId - 1 * currentExplosionRange)%15 > -1 && checkPlayerGrid(bombPositionId - 1 * currentExplosionRange) != -1){
				switch (currentExplosionRange){
					case 1: bomb->left = Player_Object; break;
					case 2: bomb->left2 = Player_Object; break;
				}
		}
	}
}
	
	// Check if right has player
	for (currentExplosionRange = 1; currentExplosionRange < getPowerExplosionRange(bomb) + 1; currentExplosionRange ++){
		if (getCharacterType(bomb, right, currentExplosionRange) == None){
			if ((bombPositionId + 1 * currentExplosionRange)%15 < 15 && checkPlayerGrid(bombPositionId + 1 * currentExplosionRange) != -1){
				switch (currentExplosionRange){
					case 1: bomb->right = Player_Object; break;
					case 2: bomb->right2 = Player_Object; break;
				}
		}
	}
}
	
	// Check if top has powerup
	for (currentExplosionRange = 1; currentExplosionRange < getPowerExplosionRange(bomb) + 1; currentExplosionRange ++){
		if (getCharacterType(bomb, up, currentExplosionRange) == None){
			if ((((bombPositionId - (15 * currentExplosionRange))/15) > -1 )&& (containsPowerUpByPositionId(bombPositionId - (15 * currentExplosionRange)) != NoPowerUp) && (getBlockType(bombPositionId - 15 * currentExplosionRange) != SoftBlock)){
				switch (currentExplosionRange){
					case 1: bomb->up = PowerUp_Object; break;
					case 2: bomb->up2 = PowerUp_Object; break;
				}
			}
		}
	}
		
	// Check if bottom has powerup
	for (currentExplosionRange = 1; currentExplosionRange < getPowerExplosionRange(bomb) + 1; currentExplosionRange ++){
		if (getCharacterType(bomb, down, currentExplosionRange) == None){
			if ((bombPositionId + 15 * currentExplosionRange)/15 < 15 && containsPowerUpByPositionId(bombPositionId + 15 * currentExplosionRange) != NoPowerUp && (getBlockType(bombPositionId + 15 * currentExplosionRange) != SoftBlock)){
				switch (currentExplosionRange){
					case 1: bomb->down = PowerUp_Object; break;
					case 2: bomb->down2 = PowerUp_Object; break;
				}
			}
		}
	}
		
		
	// Check if left has powerup
	for (currentExplosionRange = 1; currentExplosionRange < getPowerExplosionRange(bomb) + 1; currentExplosionRange ++){
		if (getCharacterType(bomb, left, currentExplosionRange) == None){
			if ((bombPositionId -1 * currentExplosionRange)%15 > -1 && containsPowerUpByPositionId(bombPositionId -1 * currentExplosionRange) != NoPowerUp && (getBlockType(bombPositionId - 1 * currentExplosionRange) != SoftBlock)){
				switch (currentExplosionRange){
					case 1: bomb->left = PowerUp_Object; break;
					case 2: bomb->left2 = PowerUp_Object; break;
				}
			}
		}
	}
		
	// Check if right has powerup
	for (currentExplosionRange = 1; currentExplosionRange < getPowerExplosionRange(bomb) + 1; currentExplosionRange ++){
		if (getCharacterType(bomb, right, currentExplosionRange) == None){
			if ((bombPositionId + 1 * currentExplosionRange)%15 < 15 && containsPowerUpByPositionId(bombPositionId + 1 * currentExplosionRange) != NoPowerUp && (getBlockType(bombPositionId + 1 * currentExplosionRange) != SoftBlock)){
				switch (currentExplosionRange){
					case 1: bomb->right = PowerUp_Object; break;
					case 2: bomb->right2 = PowerUp_Object; break;
				}
			}
		}
	}

	// Check if top has block
	for (currentExplosionRange = 1; currentExplosionRange < getPowerExplosionRange(bomb) + 1; currentExplosionRange ++){
		if (getCharacterType(bomb, up, currentExplosionRange) == None){
			if ((bombPositionId - 15 * currentExplosionRange)/15 > -1 && (getBlockType(bombPositionId - 15 * currentExplosionRange) == SoftBlock || getBlockType(bombPositionId - 15 * currentExplosionRange) == GrassBlock)){
				switch (currentExplosionRange){
					case 1: bomb->up = Block_Object; break;
					case 2: bomb->up2 = Block_Object; break;
				}
				
				//bomb->up = Block_Object;
				Block * block = getBlock(bombPositionId - 15 * currentExplosionRange);
				block->blockType = GrassBlock;
				block->blockImage = Grass_block;
				UpdateGridBlock(block);
		}
	}
}
	
	// Check if bottom has block
	for (currentExplosionRange = 1; currentExplosionRange < getPowerExplosionRange(bomb) + 1; currentExplosionRange ++){
		if (getCharacterType(bomb, down, currentExplosionRange) == None){
			if ((bombPositionId + 15 * currentExplosionRange)/15 < 15 && (getBlockType(bombPositionId + 15 * currentExplosionRange) == SoftBlock || getBlockType(bombPositionId + 15 * currentExplosionRange) == GrassBlock)){
				switch (currentExplosionRange){
					case 1: bomb->down = Block_Object; break;
					case 2: bomb->down2 = Block_Object; break;
				}
				
				//bomb->down = Block_Object;
				Block * block = getBlock(bombPositionId + 15 * currentExplosionRange);
				block->blockType = GrassBlock;
				block->blockImage = Grass_block;
				UpdateGridBlock(block);
		}
	}
}
	// Check if left has block
	for (currentExplosionRange = 1; currentExplosionRange < getPowerExplosionRange(bomb) + 1; currentExplosionRange ++){
		if (getCharacterType(bomb, left, currentExplosionRange) == None){
			if ((bombPositionId - 1 * currentExplosionRange)%15 > -1 && (getBlockType(bombPositionId - 1 * currentExplosionRange) == SoftBlock || getBlockType(bombPositionId - 1 * currentExplosionRange) == GrassBlock)){
				switch (currentExplosionRange){
					case 1: bomb->left = Block_Object; break;
					case 2: bomb->left2 = Block_Object; break;
				}
				
				//bomb->left = Block_Object;
				Block * block = getBlock(bombPositionId - 1 * currentExplosionRange);
				block->blockType = GrassBlock;
				block->blockImage = Grass_block;
				UpdateGridBlock(block);
		}
	}
}
	// Check if right has block
	for (currentExplosionRange = 1; currentExplosionRange < getPowerExplosionRange(bomb) + 1; currentExplosionRange ++){
		if (getCharacterType(bomb, right, currentExplosionRange) == None){
			if ((bombPositionId + 1 * currentExplosionRange)%15 < 15 && (getBlockType(bombPositionId + 1 * currentExplosionRange) == SoftBlock || getBlockType(bombPositionId + 1 * currentExplosionRange) == GrassBlock)){
				switch (currentExplosionRange){
					case 1: bomb->right = Block_Object; break;
					case 2: bomb->right2 = Block_Object; break;
				}
				
				//bomb->right = Block_Object;
				Block * block = getBlock(bombPositionId + 1 * currentExplosionRange);
				block->blockType = GrassBlock;
				block->blockImage = Grass_block;
				UpdateGridBlock(block);
		}
	}
}
	
	
	// Check if top has bomb
	for (currentExplosionRange = 1; currentExplosionRange < getPowerExplosionRange(bomb) + 1; currentExplosionRange ++){
		if (getCharacterType(bomb, up, currentExplosionRange) == None){
			if ((bombPositionId - 15 * currentExplosionRange)/15 > -1 && checkBombGrid(bombPositionId - 15 * currentExplosionRange) != -1 && checkExplosionStatus(bombPositionId - 15 * currentExplosionRange) == notExploded){
				switch (currentExplosionRange){
					case 1: bomb->up = Bomb_Object; break;
					case 2: bomb->up2 = Bomb_Object; break;
				}
				
				
				//bomb->up = Bomb_Object;
					Bomb_RayTrace(inventory, player, getBombFromListWithPosition(inventory, bombPositionId - 15 * currentExplosionRange), bombPositionId - 15 * currentExplosionRange);
				
			}
		}
	}
	
	// Check if bottom has bomb
	for (currentExplosionRange = 1; currentExplosionRange < getPowerExplosionRange(bomb) + 1; currentExplosionRange ++){
		if (getCharacterType(bomb, down, currentExplosionRange) == None){
			if ((bombPositionId + 15 * currentExplosionRange)/15 < 15 && checkBombGrid(bombPositionId + 15 * currentExplosionRange) != -1 && checkExplosionStatus(bombPositionId + 15 * currentExplosionRange) == notExploded){
				switch (currentExplosionRange){
					case 1: bomb->down = Bomb_Object; break;
					case 2: bomb->down2 = Bomb_Object; break;
				}
				//bomb->down = Bomb_Object;
		
					Bomb_RayTrace(inventory, player, getBombFromListWithPosition(inventory, bombPositionId + 15 * currentExplosionRange), bombPositionId + 15 * currentExplosionRange);
				
			}
		}
	}
	// Check if left has bomb
	for (currentExplosionRange = 1; currentExplosionRange < getPowerExplosionRange(bomb) + 1; currentExplosionRange ++){
		if (getCharacterType(bomb, left, currentExplosionRange) == None){
			if ((bombPositionId - 1 * currentExplosionRange)%15 > -1 && checkBombGrid(bombPositionId - 1 * currentExplosionRange) != -1 && checkExplosionStatus(bombPositionId - 1 * currentExplosionRange) == notExploded){
				switch (currentExplosionRange){
					case 1: bomb->left = Bomb_Object; break;
					case 2: bomb->left2 = Bomb_Object; break;
				}
				
				//bomb->left = Bomb_Object;

					Bomb_RayTrace(inventory, player, getBombFromListWithPosition(inventory, bombPositionId - 1 * currentExplosionRange), bombPositionId - 1 * currentExplosionRange);
				
			}
		}
	}
	
	// Check if right has bomb
	for (currentExplosionRange = 1; currentExplosionRange < getPowerExplosionRange(bomb) + 1; currentExplosionRange ++){
		if (getCharacterType(bomb, right, currentExplosionRange) == None){
			if ((bombPositionId + 1 * currentExplosionRange)%15 < 15 && checkBombGrid(bombPositionId + 1 * currentExplosionRange) != -1 && checkExplosionStatus(bombPositionId + 1 * currentExplosionRange) == notExploded){
				switch (currentExplosionRange){
					case 1: bomb->right = Bomb_Object; break;
					case 2: bomb->right2 = Bomb_Object; break;
				}
				//bomb->right = Bomb_Object;
			
					Bomb_RayTrace(inventory, player, getBombFromListWithPosition(inventory, bombPositionId + 1 * currentExplosionRange), bombPositionId + 1 * currentExplosionRange);
				
			}
		}
	}
}
/*~~~~~~~~~~~~get Explosion Range: Depends if Player is within range of explosion (1 pixel in)~~~~~~~~~~~~~~~*/
Range getExplosionRange(Player * player, int16_t explosionXPosition, int16_t explosionYPosition){
	int16_t overlapX = abs(player->xPosition - explosionXPosition);
	int16_t overlapY = abs(player->yPosition - explosionYPosition);
	if (overlapX < 8 && overlapY < 8){
		return inRange;
	}
	//else if (player->playerPowerUp == PowerFire && overlapX < 16 && overlapY < 16){
	//	return inRange;
	//}
	else{
		return outOfRange;
	}
}
/*~~~~~~~~~~~~~~~~~set Power Explosion Range~~~~~~~~~~~~~~~~~~~~~~~~*/
void setPowerExplosionRange(Bombs * bomb, int16_t newBombRange, Inventory * inventory){
	bomb->rangeOfExplosion = newBombRange;
} 
int16_t getPowerExplosionRange(Bombs * bomb){
	return bomb->rangeOfExplosion; 
}
/*~~~~~~~~~~~~Update Bomb Explosion: Updates explosion, removes explosion, detects living state, and uncovers PowerUps~~~~~~~~~~~~~`*/
void UpdateBombExplosion(Player * currentPlayer1, Player * currentPlayer2,  Bombs * bomb){
	
if (bomb->hasExploded == exploded){
	
	RemoveExplosion(bomb, bomb->positionId, standing);
	
		
	Timer1A_Stop();
	Timer3A_Stop();
	
	determineStateOfLiving(currentPlayer1, currentPlayer2, bomb->positionId);
	
	if (bomb->up != None){
		RemoveExplosion(bomb, bomb->positionId, up);
		determineStateOfLiving(currentPlayer1, currentPlayer2, bomb->positionId - 15);
	}
	if (bomb->up2 != None){
		determineStateOfLiving(currentPlayer1, currentPlayer2, bomb->positionId - 30);
	}	
	if (bomb->down != None){
		RemoveExplosion(bomb, bomb->positionId, down);
		determineStateOfLiving(currentPlayer1, currentPlayer2, bomb->positionId + 15);
	}
	if (bomb->down2 != None){
		determineStateOfLiving(currentPlayer1, currentPlayer2, bomb->positionId + 30);
	}	
	if (bomb->left != None){
		RemoveExplosion(bomb, bomb->positionId, left);
		determineStateOfLiving(currentPlayer1, currentPlayer2, bomb->positionId - 1);
	}
	if (bomb->left2 != None){
		determineStateOfLiving(currentPlayer1, currentPlayer2, bomb->positionId - 2);
	}	
	if (bomb->right != None){
		RemoveExplosion(bomb, bomb->positionId, right);
		determineStateOfLiving(currentPlayer1, currentPlayer2, bomb->positionId + 1);
	} 
	if (bomb->right2 != None){
		determineStateOfLiving(currentPlayer1, currentPlayer2, bomb->positionId + 2);
	} 	
}

else{
	
	Update_Sprite(&bomb->explosionCenter);
	DrawExplosion(bomb, bomb->positionId, standing);
	
		
	Sound_Play(3);
	Timer1A_Start();
	Timer3A_Start();
	
	if (bomb->up != None){
		//Update_Sprite(&bomb->explosionUp);
		if (getBlockType(bomb->positionId - 15) == HardBlock){
			bomb->up2 = None;
		}
		DrawExplosion(bomb, bomb->positionId, up);
		

	}
	if (bomb->down != None){
		//Update_Sprite(&bomb->explosionDown);
		if (getBlockType(bomb->positionId + 15) == HardBlock){
			bomb->down2 = None;
		}
		DrawExplosion(bomb, bomb->positionId, down);
	}
	
	if (bomb->left != None){
		//Update_Sprite(&bomb->explosionLeft);
		if (getBlockType(bomb->positionId - 1) == HardBlock){
			bomb->left2 = None;
		}
		DrawExplosion(bomb, bomb->positionId, left);
	}
	
	if (bomb->right != None){
		//Update_Sprite(&bomb->explosionRight);
		if (getBlockType(bomb->positionId + 1) == HardBlock){
			bomb->right2 = None;
		}
		DrawExplosion(bomb, bomb->positionId, right);
	}
}
}
/*~~~~~~~~~~~~~~Init_ObjectGrid: Allocated Matrix to determine which Position ID's have explosions~~~~~~~~~~~~~~~~~*/
void Init_ObjectGrid(void){
	int16_t row;
	int16_t col;
	for (row = 0; row < 15; row++){
		for (col = 0; col < 15; col++){
			objectGrid[row][col].xPosition = 4 + 8 * col;
			objectGrid[row][col].yPosition = 44 + 8 * row;
			objectGrid[row][col].playerId = -1;
			objectGrid[row][col].bombId = -1;
			objectGrid[row][col].positionId = row * 15 + col;
			objectGrid[row][col].hasExploded = notExploded;
			objectGrid[row][col].bombPlayer = -1;
		}
	}
	
	playerPrevious1 = objectGrid[1][1].positionId;
	playerPrevious2 = objectGrid[14][14].positionId;
}
/*~~~~~~~~~~~~~~Update Player Grid: Allocated matrix for location if Player exists in square~~~~~~~~~~~~~~*/
void UpdatePlayerGrid(Player * player){
	int16_t row = player->playerPositionId / 15;
	int16_t col = player->playerPositionId % 15;
	int16_t rowPrevious;
	int16_t colPrevious;
	if (player->playerId == 1){
		rowPrevious = playerPrevious1 / 15;
		colPrevious = playerPrevious1 % 15;
		objectGrid[rowPrevious][colPrevious].playerId = -1;
		objectGrid[row][col].playerId = player->playerId;
		playerPrevious1 = player->playerPositionId;
	}
	else{
		rowPrevious = playerPrevious2 / 15;
		colPrevious = playerPrevious2 % 15;
		objectGrid[rowPrevious][colPrevious].playerId = -1;
		objectGrid[row][col].playerId = player->playerId;
		playerPrevious2 = player->playerPositionId;
	}
}
	
int16_t getBombPlayer(int16_t bombPositionId){
	int16_t row = bombPositionId / 15;
	int16_t col = bombPositionId % 15;
	return objectGrid[row][col].bombPlayer;
}

int16_t checkBombGrid(int16_t bombPositionId){
	int16_t row = bombPositionId / 15;
	int16_t col = bombPositionId % 15;
	return objectGrid[row][col].bombId;
}

int16_t checkPlayerGrid(int16_t playerPositionId){
	int16_t row = playerPositionId / 15;
	int16_t col = playerPositionId % 15;
	return objectGrid[row][col].playerId;
}
	
void UpdateBombGrid(Bombs * bomb){
	int16_t row = bomb->positionId / 15;
	int16_t col = bomb->positionId % 15;
	objectGrid[row][col].bombId = bomb->bombId;
	objectGrid[row][col].bombPlayer = bomb->bombPlayer;
}

void UpdateBombStatusGrid(Bombs * bomb, ExplosionStatus explosionStatus){
	int16_t row = bomb->positionId / 15;
	int16_t col = bomb->positionId % 15;
	objectGrid[row][col].hasExploded = explosionStatus;
	
}


void RemoveBombFromGrid(Bombs * bomb){
	int16_t row = bomb->positionId / 15;
	int16_t col = bomb->positionId % 15;
	objectGrid[row][col].bombId = -1;
	objectGrid[row][col].bombId = -1;
	objectGrid[row][col].bombPlayer = -1;
}

ExplosionStatus checkExplosionStatus(int16_t bombPositionId){
	int16_t row = bombPositionId / 15;
	int16_t col = bombPositionId % 15;
	return objectGrid[row][col].hasExploded;
}

int16_t getExplosionXPosition(int16_t bombPositionId){
	int16_t row = bombPositionId / 15;
	int16_t col = bombPositionId % 15;
	return objectGrid[row][col].xPosition;
}

int16_t getExplosionYPosition(int16_t bombPositionId){
	int16_t row = bombPositionId / 15;
	int16_t col = bombPositionId % 15;
	return objectGrid[row][col].yPosition;

}
	
/*~~~~~~~~~Draw Explosion~~~~~~~*/
void DrawExplosion(Bombs * bomb, int16_t bombPositionId, Direction direction){
	
	if (direction == standing){
	ST7735_DrawBitmap(getExplosionXPosition(bombPositionId), getExplosionYPosition(bombPositionId), getFrame(&(bomb->explosionCenter)), 8, 8);
		if (getAnimationStatus(&bomb->explosionCenter) == Done){
				bomb->hasExploded = exploded;
				UpdateBombStatusGrid(bomb, exploded);
			}
		return;
		}
	if (direction == up){
	ST7735_DrawBitmap(getExplosionXPosition(bombPositionId - 15), getExplosionYPosition(bombPositionId - 15), getFrame(&(bomb->explosionCenter)), 8, 8);
		if (bomb->up2 != None){
			ST7735_DrawBitmap(getExplosionXPosition(bombPositionId - 30), getExplosionYPosition(bombPositionId - 30), getFrame(&(bomb->explosionCenter)), 8, 8);
		}
		
		return;
		}
	
if (direction == down){
	ST7735_DrawBitmap(getExplosionXPosition(bombPositionId + 15), getExplosionYPosition(bombPositionId + 15), getFrame(&(bomb->explosionCenter)), 8, 8);
		if (bomb->down2 != None){
			ST7735_DrawBitmap(getExplosionXPosition(bombPositionId + 30), getExplosionYPosition(bombPositionId + 30), getFrame(&(bomb->explosionCenter)), 8, 8);
		}
		return;
		}

if (direction == left){
	ST7735_DrawBitmap(getExplosionXPosition(bombPositionId - 1), getExplosionYPosition(bombPositionId - 1), getFrame(&(bomb->explosionCenter)), 8, 8);
	if (bomb->left2 != None){
			ST7735_DrawBitmap(getExplosionXPosition(bombPositionId - 2), getExplosionYPosition(bombPositionId - 2), getFrame(&(bomb->explosionCenter)), 8, 8);
		}
	return;
	}

if (direction == right){
	ST7735_DrawBitmap(getExplosionXPosition(bombPositionId + 1), getExplosionYPosition(bombPositionId + 1), getFrame(&(bomb->explosionCenter)), 8, 8);
	if (bomb->right2 != None){
			ST7735_DrawBitmap(getExplosionXPosition(bombPositionId + 2), getExplosionYPosition(bombPositionId + 2), getFrame(&(bomb->explosionCenter)), 8, 8);
		}
	return;
	}

}

void RemoveExplosion(Bombs * bomb, int16_t bombPositionId, Direction direction){
	if (direction == standing){
		ST7735_FillRect(getExplosionXPosition(bombPositionId), getExplosionYPosition(bombPositionId) - 7, 8, 8, ST7735_Color565(36, 108, 0));
		return;
	}
	if (direction == up){
		ST7735_FillRect(getExplosionXPosition(bombPositionId - 15), getExplosionYPosition(bombPositionId -15) - 7, 8, 8, ST7735_Color565(36, 108, 0));
		if (bomb->up2 != None){
			ST7735_FillRect(getExplosionXPosition(bombPositionId - 30), getExplosionYPosition(bombPositionId -30) - 7, 8, 8, ST7735_Color565(36, 108, 0));
		}
		if (bomb->up == Block_Object && getBlock(bombPositionId - 15)->powerUp != NoPowerUp){
			setBlockHiddenStatus(uncovered, bombPositionId - 15);
		}
		if (bomb->up2 == Block_Object && getBlock(bombPositionId - 30)->powerUp != NoPowerUp){
			setBlockHiddenStatus(uncovered, bombPositionId - 30);
		}
		return;
	}
	if (direction == down){
		ST7735_FillRect(getExplosionXPosition(bombPositionId + 15), getExplosionYPosition(bombPositionId + 15) - 7, 8, 8, ST7735_Color565(36, 108, 0));
		if (bomb->down2 != None){
			ST7735_FillRect(getExplosionXPosition(bombPositionId + 30), getExplosionYPosition(bombPositionId + 30) - 7, 8, 8, ST7735_Color565(36, 108, 0));
		}
		if (bomb->down == Block_Object && getBlock(bombPositionId + 15)->powerUp != NoPowerUp){
			setBlockHiddenStatus(uncovered, bombPositionId + 15);
		}
		if (bomb->down2 == Block_Object && getBlock(bombPositionId + 30)->powerUp != NoPowerUp){
			setBlockHiddenStatus(uncovered, bombPositionId + 30);
		}
		return;
	}
	if (direction == left){
		ST7735_FillRect(getExplosionXPosition(bombPositionId - 1), getExplosionYPosition(bombPositionId - 1) - 7, 8, 8, ST7735_Color565(36, 108, 0));
		if (bomb->left2 != None){
			ST7735_FillRect(getExplosionXPosition(bombPositionId - 2), getExplosionYPosition(bombPositionId - 2) - 7, 8, 8, ST7735_Color565(36, 108, 0));
		}
		if (bomb->left == Block_Object && getBlock(bombPositionId - 1)->powerUp != NoPowerUp){
			setBlockHiddenStatus(uncovered, bombPositionId - 1);
		}
		if (bomb->left2 == Block_Object && getBlock(bombPositionId - 2)->powerUp != NoPowerUp){
			setBlockHiddenStatus(uncovered, bombPositionId - 2);
		}
		return;
	}
	if (direction == right){
		ST7735_FillRect(getExplosionXPosition(bombPositionId + 1), getExplosionYPosition(bombPositionId + 1) - 7, 8, 8, ST7735_Color565(36, 108, 0));
		if (bomb->right2 != None){
			ST7735_FillRect(getExplosionXPosition(bombPositionId + 2), getExplosionYPosition(bombPositionId + 2) - 7, 8, 8, ST7735_Color565(36, 108, 0));
		}
		if (bomb->right == Block_Object && getBlock(bombPositionId + 1)->powerUp != NoPowerUp){
			setBlockHiddenStatus(uncovered, bombPositionId + 1);
		}
		if (bomb->right2 == Block_Object && getBlock(bombPositionId + 2)->powerUp != NoPowerUp){
			setBlockHiddenStatus(uncovered, bombPositionId + 2);
		}
		return;
	}
}
