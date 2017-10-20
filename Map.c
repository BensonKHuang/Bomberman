// Map.c
// Runs on LM4F120/TM4C123
// Brian Tsai, Benson Huang
// Last Modified: 05/04/2017
#include <stdint.h>
#include <stdlib.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
#include "Map.h"
#include "PowerUpImages.h"
#include "PowerUps.h"
#include "Player.h"

int16_t row;
int16_t col;
int16_t rowUp;
int16_t rowDown;
int16_t colLeft;
int16_t colRight;
int16_t blockNumber;
int8_t upBlock;
int8_t downBlock;
int8_t leftBlock;
int8_t rightBlock;
int8_t currentDirection;



const unsigned short Solid_block[] = {
 0x7BEF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x7BEF, 0xEF5D, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x0000,
 0xEF5D, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x0000, 0xEF5D, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x0000,
 0xEF5D, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x0000, 0xEF5D, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x0000,
 0xEF5D, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x0000, 0xC618, 0xEF5D, 0xEF5D, 0xEF5D, 0xEF5D, 0xEF5D, 0xEF5D, 0x7BEF,

};

const unsigned short Brick_block[] = {
 0x52AA, 0x52AA, 0x8C51, 0xC618, 0x0000, 0x8C51, 0x52AA, 0x52AA, 0x52AA, 0x52AA, 0x52AA, 0x8C51, 0x0000, 0x52AA, 0x8C51, 0x8C51,
 0x8C51, 0x52AA, 0x8C51, 0xC618, 0x0000, 0x52AA, 0x52AA, 0x8C51, 0x0000, 0x0000, 0x0000, 0x0000, 0x4208, 0x0000, 0x0000, 0x0000,
 0x8C51, 0x52AA, 0x52AA, 0x52AA, 0x52AA, 0x8C51, 0xC618, 0x0000, 0x52AA, 0x8C51, 0x8C51, 0x52AA, 0x52AA, 0x52AA, 0x8C51, 0x0000,
 0x52AA, 0x52AA, 0x8C51, 0x8C51, 0x52AA, 0x8C51, 0xC618, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4208,

};

const unsigned short Grass_block[] = {
 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364,
 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364,
 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364,
 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364, 0x0364,

};


Block helper;

//matrices
Block blockGrid[15][15];
/*~~~~~~~~~~~~~~~Map_Init() builds the map and layout with RNG~~~~~~~~~~~~~~~~~~~*/
void Map_Init(Supply * supply){
	
	Perimeter();
	InnerHardBlocks();
	Grass();
	RandomizeCorners();
	SoftBlockRandomizer();
	IdBlocks();
	PowerUps_Init(supply);
	DrawGrid();
			
}
	/*~~~~~~~~~~~~~~~Perimeter Places perimeter of Hard Blocks~~~~~~~~~~~~~~~~~~~*/
void Perimeter(void){
	
	// Fill leftmost column
	for(int row = 0; row < 15; row ++ ){
			blockGrid[row][0].xPosition = 4;
			blockGrid[row][0].yPosition = 44 + 8 * row;
		  blockGrid[row][0].canOccupy = occupied;
			blockGrid[row][0].size = 8;
			blockGrid[row][0].blockImage = Solid_block;
			blockGrid[row][0].xPositionCenter = 4 + 4;
			blockGrid[row][0].yPositionCenter = (44 + 8 * row) - 4;
			blockGrid[row][0].blockType = HardBlock;
	}
	
	// Fill rightmost column
	for(int row = 0; row < 15; row ++ ){
			blockGrid[row][14].xPosition = 116;
			blockGrid[row][14].yPosition = 44 + 8 * row;
		  blockGrid[row][14].canOccupy = occupied;
			blockGrid[row][14].size = 8;
			blockGrid[row][14].blockImage = Solid_block;
			blockGrid[row][14].xPositionCenter = 116 + 4;
			blockGrid[row][14].yPositionCenter = (44 + 8 * row) - 4;
			blockGrid[row][14].blockType = HardBlock;
	}
	
			
	// Fill top row	
	for (int col = 0; col < 15; col ++ ){
			blockGrid[0][col].xPosition = 4 + 8 * col;
			blockGrid[0][col].yPosition = 44;
			blockGrid[0][col].canOccupy = occupied;
		  blockGrid[0][col].size = 8;
			blockGrid[0][col].blockImage = Solid_block;
			blockGrid[0][col].xPositionCenter = (4 + 8 * col) + 4;
			blockGrid[0][col].yPositionCenter = 44 - 4;
			blockGrid[0][col].blockType = HardBlock;
	}
		
		
  // Fill bottom row
	for (int col = 0; col < 15; col ++ ){
			blockGrid[14][col].xPosition = 4 + 8 * col;
			blockGrid[14][col].yPosition = 156;
			blockGrid[14][col].canOccupy = occupied;
			blockGrid[14][col].size = 8;
			blockGrid[14][col].blockImage = Solid_block;
			blockGrid[14][col].xPositionCenter = (4 + 8 * col) + 4;
			blockGrid[14][col].yPositionCenter = 156 - 4;
			blockGrid[14][col].blockType = HardBlock;
	}
}
/*~~~~~~~~~~~~~~~InnerHardBlocks: Places Hard Blocks to set up Grid for Map~~~~~~~~~~~~~~~~~~~*/
void InnerHardBlocks(void){
	
	for (int row = 2; row < 14; row +=2 ){
		for ( int col = 2; col < 14; col+=2 ){
			blockGrid[row][col].xPosition = 20 + 8 * (col - 2);
			blockGrid[row][col].yPosition = 60 + 8 * (row - 2);
			blockGrid[row][col].canOccupy = occupied;
			blockGrid[row][col].size = 8;
			blockGrid[row][col].blockImage = Solid_block;
			blockGrid[row][col].xPositionCenter = (20 + 8 * (col - 2)) + 4;
			blockGrid[row][col].yPositionCenter = (60 + 8 * (row - 2)) - 4;
			blockGrid[row][col].blockType = HardBlock;
		}
	}
}
/*~~~~~~~~~~~~~~~Grass: Placed in free spaces~~~~~~~~~~~~~~~~~~~*/
void Grass(void){
	for(int row = 0; row < 15; row++ ){
		for (int col = 0; col < 15; col++ ){
			if (blockGrid[row][col].canOccupy != occupied)
			{
				blockGrid[row][col].xPosition = 4 + 8 * col;
				blockGrid[row][col].yPosition = 44 + 8 * row;
				blockGrid[row][col].canOccupy = unoccupied;
			  blockGrid[row][col].size = 8;
				blockGrid[row][col].blockImage = Grass_block;
				blockGrid[row][col].xPositionCenter = (4 + 8 * col) + 4;
				blockGrid[row][col].yPositionCenter = (44 + 8 * row) - 4;
				blockGrid[row][col].blockType = GrassBlock;
			}
		}
	}
}
/*~~~~~~~~~~~~~~~RandomizeCorners~~~~~~~~~~~~~~~~~~~*/

void RandomizeCorners(void){
	int x;
	int y;
	int cornerOrientation = RandomCornerOrientation();;

//	if (cornerOrientation == 0){
	
		// Top left corner
		for (x = 1; x < 4; x++){
			blockGrid[1][x].canOccupy = occupied;
		}
	
		for (y = 1; y < 4; y++){
			blockGrid[y][1].canOccupy = occupied;
		}
	
	// Bottom right corner
	  for (x = 13; x >=11; x--) {
		  blockGrid[13][x].canOccupy = occupied;
	  }
	  for (y = 13; y >= 11; y--) {
	  	blockGrid[y][13].canOccupy = occupied;
	  }
  //}
	
/*	if (cornerOrientation == 1){
	// Top right corner
	  for (x = 13; x >=11; x--) {
		  blockGrid[1][x].canOccupy = occupied;
	  }
	  for (y = 1; y < 4; y++){
	  	blockGrid[y][13].canOccupy = occupied;
	  }
	

	//Bottom left corner
	  for (x = 1; x < 4; x++){
	  	blockGrid[13][x].canOccupy = occupied;
	  }
	  for (y = 13; y >= 11; y--) {
	  	blockGrid[y][1].canOccupy = occupied;
	  }
  } */
}
	
	
	
/*~~~~~~~~~~~~~~~SoftBlockRandomizer randomizes Soft Block Generation based on seed ~~~~~~~~~~~~~~~~~~~*/

void SoftBlockRandomizer(void){
	
	int SoftBlocks = RandomSoftBlocks();
	//int SoftBlocks = 20;
	int row;
	int col;
	while(SoftBlocks != 0){
		row = RandomGridPosition();
		col = RandomGridPosition();
		if (blockGrid[row][col].canOccupy !=occupied){
				blockGrid[row][col].canOccupy = occupied;
				blockGrid[row][col].blockImage = Brick_block;
				blockGrid[row][col].blockType = SoftBlock;
				SoftBlocks --;
		}
	}
	
}

int RandomGridPosition(void){
	//Random_Init(NVIC_ST_CURRENT_R);
	return((Random32()>>24)%13) + 1;

}

int RandomSoftBlocks(void){
	//Random_Init(NVIC_ST_CURRENT_R);
	//return((Random32()>>24)%11)+110;
	return((Random32()>>24)%11)+80;
}

int RandomCornerOrientation(void) {
	//Random_Init(NVIC_ST_CURRENT_R);
	return((Random32()>>24)%2);

}

int16_t getBlockXPosition(Block * block){
	return block->xPosition;
}

int16_t getBlockYPosition(Block * block){
	return block->yPosition;
}

int16_t getBlockXPositionCenter(Block * block){
	return block->xPositionCenter;
}

int16_t getBlockYPositionCenter(Block * block){
	return block->yPositionCenter;
}

Block* getBlock(int16_t positionId){
	int row = positionId / 15;
	int col = positionId % 15;
	return &blockGrid[row][col];
}

Block* getTopBlock(int16_t positionId){
	int row = positionId / 15;
	int col = positionId % 15;
	return &blockGrid[row - 1][col];
}

Block* getBottomBlock(int16_t positionId){
	int row = positionId / 15;
	int col = positionId % 15;
	return &blockGrid[row + 1][col];
}

Block* getLeftBlock(int16_t positionId){
	int row = positionId / 15;
	int col = positionId % 15;
	return &blockGrid[row][col - 1];
}

Block* getRightBlock(int16_t positionId){
	int row = positionId / 15;
	int col = positionId % 15;
	return &blockGrid[row][col + 1];
}


int16_t getBlockId(Block * block){
	return block->idNumber;
}

Type getBlockType(int16_t targetPositionId){
	int row = targetPositionId / 15;
	int col = targetPositionId % 15;
	return blockGrid[row][col].blockType;
}


int16_t getBlockXPositionById(int16_t targetPositionId){
	int row = targetPositionId / 15;
	int col = targetPositionId % 15;
	return blockGrid[row][col].xPosition;
}

int16_t getBlockYPositionById(int16_t targetPositionId){
	int row = targetPositionId / 15;
	int col = targetPositionId % 15;
	return blockGrid[row][col].yPosition;
}
	
void UpdateGridBlock(Block * block){
	int row = block->idNumber / 15;
	int col = block->idNumber % 15;
	blockGrid[row][col] = *block;
}

void IdBlocks(void){
	int id = 0;
	for (int row = 0; row < 15; row ++){
		for (int col = 0; col < 15; col++){
			blockGrid[row][col].idNumber = id;
			blockGrid[row][col].powerUp = NoPowerUp;
			id++;
		}
	}
}

void DrawGrid(void){
		for (int row = 0; row < 15; row ++){
		for (int col = 0; col < 15; col++){
			if (blockGrid[row][col].blockType == GrassBlock && blockGrid[row][col].powerUp != NoPowerUp){
					ST7735_DrawBitmap(blockGrid[row][col].xPosition, blockGrid[row][col].yPosition, blockGrid[row][col].powerUpImage, blockGrid[row][col].size, blockGrid[row][col].size);
			}
			else{
				  ST7735_DrawBitmap(blockGrid[row][col].xPosition, blockGrid[row][col].yPosition, blockGrid[row][col].blockImage, blockGrid[row][col].size, blockGrid[row][col].size);
		}
	}
	} 
}
void ClearGrid(void){
	for (int row = 0; row < 15; row ++){
		for (int col = 0; col < 15; col++){
		blockGrid[row][col].canOccupy = unoccupied;
		blockGrid[row][col].blockType = GrassBlock;	
		blockGrid[row][col].hiddenStatus = uncovered;
		blockGrid[row][col].blockImage = Grass_block;
		blockGrid[row][col].powerUp = NoPowerUp;
		}
	}
}

PowerUpType containsPowerUpByPositionId(int16_t targetPowerUpPosition){
	int row = targetPowerUpPosition / 15;
	int col = targetPowerUpPosition % 15;
	return blockGrid[row][col].powerUp;
}

PowerUpType containsPowerUpByBlock(Block * block){
	int row = block->idNumber / 15;
	int col = block->idNumber % 15;
	return blockGrid[row][col].powerUp;
}

void UncoverPowerUp(Block * block){
	int row = block->idNumber / 15;
	int col = block->idNumber % 15;
	ST7735_DrawBitmap(blockGrid[row][col].xPosition, blockGrid[row][col].yPosition, blockGrid[row][col].powerUpImage, blockGrid[row][col].size, blockGrid[row][col].size);
}
/*~~~~~~~~~~~~~~~setPowerUp: Places Powerups in Area~~~~~~~~~~~~~~~~~~~*/

void setPowerUp(PowerUpType powerUp, int16_t positionId){
	int row = positionId / 15;
	int col = positionId % 15;
	blockGrid[row][col].powerUp = powerUp;
	switch (powerUp){
		case PowerBomb: blockGrid[row][col].powerUpImage = Power_Bomb; 
		     if (blockGrid[row][col].blockType == GrassBlock){
					 blockGrid[row][col].hiddenStatus = uncovered; break;
				 }
				 else {
					 blockGrid[row][col].hiddenStatus = covered; break;
				 }
		case PowerFire: blockGrid[row][col].powerUpImage = Power_Fire;
				 if (blockGrid[row][col].blockType == GrassBlock){
					 blockGrid[row][col].hiddenStatus = uncovered; break;
				 }
				 else {
					 blockGrid[row][col].hiddenStatus = covered; break;
				 }
		case PowerSkate: blockGrid[row][col].powerUpImage = Power_Skate;
				 if (blockGrid[row][col].blockType == GrassBlock){
					 blockGrid[row][col].hiddenStatus = uncovered; break;
				 }
				 else {
					 blockGrid[row][col].hiddenStatus = covered; break;
				 }
		case NoPowerUp: break;
	}
}
				 
Hidden getBlockHiddenStatus(int positionId){
	int row = positionId / 15;
	int col = positionId % 15;
	return blockGrid[row][col].hiddenStatus;
}

void setBlockHiddenStatus(Hidden newHiddenStatus, int positionId){
	int row = positionId / 15;
	int col = positionId % 15;
	blockGrid[row][col].hiddenStatus = newHiddenStatus;
}
		
void setBlockType(Type newBlockType, int16_t positionId){
	int row = positionId / 15;
	int col = positionId % 15;
	switch (newBlockType){
		case GrassBlock: blockGrid[row][col].blockType = GrassBlock;
									   blockGrid[row][col].blockImage = Grass_block;
				             break;
		case SoftBlock:  blockGrid[row][col].blockType = SoftBlock;
									   blockGrid[row][col].blockImage = Brick_block;
				             break;
		case HardBlock:  blockGrid[row][col].blockType = HardBlock;
									   blockGrid[row][col].blockImage = Solid_block;
				             break;
	}
}
/*~~~~~~~~~~~~~~~GenerateNewPerimeter: Generates Hard blocks around area after 100 seconds remain~~~~~~~~~~~~~~~~~~~*/
void NewPerimeter (void){
		row = 1;
		col = 1;
		rowUp = 1;
	  rowDown = 13;
		colLeft = 1;
		colRight = 13;
		blockNumber = 148;
		upBlock = 1;
	  downBlock = 2;
	  leftBlock = 3;
	  rightBlock = 4;
		currentDirection = 4;
}


void GenerateNewPerimeter(Player * currentPlayer1, Player * currentPlayer2){
		/*static int16_t row = 1;
		static int16_t col = 1;
		static int16_t rowUp = 1;
	  static int16_t rowDown = 13;
		static int16_t colLeft = 1;
		static int16_t colRight = 13;
		static int16_t blockNumber = 148;
		static int8_t up = 1;
	  static int8_t down = 2;
	  static int8_t left =3;
	  static int8_t right = 4;
		static int8_t currentDirection = 4; */
		
		if (blockNumber != 0 && col <= colRight && row == rowUp && currentDirection == rightBlock){
				if (col == colRight){
					ST7735_DrawBitmap(blockGrid[row][col].xPosition, blockGrid[row][col].yPosition, Solid_block, 8, 8); 
		      blockGrid[row][col].blockType = HardBlock;
					row++;
					currentDirection = downBlock;
			    
					blockNumber --;
				}
				else if (currentPlayer1->playerPositionId == row * 15 + col || currentPlayer2->playerPositionId == row * 15 + col){
					determineStateOfLiving(currentPlayer1, currentPlayer2, row * 15 + col);
				}
				else {
					ST7735_DrawBitmap(blockGrid[row][col].xPosition, blockGrid[row][col].yPosition, Solid_block, 8, 8);
					blockGrid[row][col].blockType = HardBlock;
					col ++;
					blockNumber --;
				}
			}
		
		else if  (blockNumber != 0 && row <= rowDown && col == colRight && currentDirection == downBlock){
			
			if (row == rowDown){
					ST7735_DrawBitmap(blockGrid[row][col].xPosition, blockGrid[row][col].yPosition, Solid_block, 8, 8);
					blockGrid[row][col].blockType = HardBlock;
				  col --;
					currentDirection = leftBlock;
					blockNumber --;
				}	
			
				else if (currentPlayer1->playerPositionId == row * 15 + col || currentPlayer2->playerPositionId == row * 15 + col){
					determineStateOfLiving(currentPlayer1, currentPlayer2, row * 15 + col);
				}
				else {
					ST7735_DrawBitmap(blockGrid[row][col].xPosition, blockGrid[row][col].yPosition, Solid_block, 8, 8);
					blockGrid[row][col].blockType = HardBlock;
					row ++;
					blockNumber --;
				}
			}
		else if  (blockNumber != 0 && col >= colLeft && row == rowDown && currentDirection == leftBlock){
			if (col == colLeft){
					ST7735_DrawBitmap(blockGrid[row][col].xPosition, blockGrid[row][col].yPosition, Solid_block, 8, 8);
				  blockGrid[row][col].blockType = HardBlock;
					row -- ;
				  currentDirection = upBlock;
					blockNumber --;
				}		
			
				else if (currentPlayer1->playerPositionId == row * 15 + col || currentPlayer2->playerPositionId == row * 15 + col){
					determineStateOfLiving(currentPlayer1, currentPlayer2, row * 15 + col);
				}
				else {
					ST7735_DrawBitmap(blockGrid[row][col].xPosition, blockGrid[row][col].yPosition, Solid_block, 8, 8);
					blockGrid[row][col].blockType = HardBlock;
					col --;
					blockNumber --;
				}
			}
			else if  (blockNumber != 0 && row >= rowUp && col == colLeft && currentDirection == upBlock){
				if (row == rowUp){
					ST7735_DrawBitmap(blockGrid[row][col].xPosition, blockGrid[row][col].yPosition, Solid_block, 8, 8);
					blockGrid[row][col].blockType = HardBlock;
					col ++;
					row ++;
          currentDirection = rightBlock;					
					blockNumber --;
					rowUp ++;
					rowDown --;
					colLeft ++;
					colRight --;
				}		
				else if (currentPlayer1->playerPositionId == row * 15 + col || currentPlayer2->playerPositionId == row * 15 + col){
					determineStateOfLiving(currentPlayer1, currentPlayer2, row * 15 + col);
				}
				else {
					ST7735_DrawBitmap(blockGrid[row][col].xPosition, blockGrid[row][col].yPosition, Solid_block, 8, 8);
					blockGrid[row][col].blockType = HardBlock;
					row --;
					blockNumber --;
				}
			}
			determineStateOfLiving(currentPlayer1, currentPlayer2, row * 15 + col);
	
}			

