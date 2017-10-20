// CollisionDetection.c
// Runs on LM4F120/TM4C123
// Checks Collision Detection for Blocks, Bombs, and PowerUps
// Brian Tsai, Benson Huang
// Last Modified: 05/04/2017

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
#include "Map.h"
#include <stdlib.h>
#include "CollisionDetection.h"
#include "Player.h"
#include "inventory.h"
#include "bomb.h"
#include "ExplosionDetection.h"
#include "PowerUps.h"


Player * testPlayer;
Block * testBlock;


/*~~~~~~~~~~~~~~~Initializes Window for Player: Determines Collision~~~~~~~~~~~~~~~~~~~*/

void Init_Window(Window * window, Player * player, int16_t playerPositionId){
	
	window->tracedPlayer = player;
	window->playerGridId = playerPositionId;
	window->topBlock = getTopBlock(playerPositionId);
	window->bottomBlock = getBottomBlock(playerPositionId);
	window->leftBlock = getLeftBlock(playerPositionId);
	window->rightBlock = getRightBlock(playerPositionId);
	
	window->topBlockId = getBlockId(window->topBlock);
	window->bottomBlockId = getBlockId(window->bottomBlock);
	window->leftBlockId = getBlockId(window->leftBlock);
	window->rightBlockId = getBlockId(window->rightBlock);
	
}
/*~~~~~~~~~~~~~~~UpdatePlayerID: Updates ID of Player Info~~~~~~~~~~~~~~~~~~~*/

void UpdatePlayerId(Window * window, Direction direction){
	int16_t playerXPosition = getXPositionCenter(window->tracedPlayer);
	int16_t playerYPosition = getYPositionCenter(window->tracedPlayer);
	
	Player * player = window->tracedPlayer;
	
	switch(direction){
		
		case standing: return; 
		
		          case up: testBlock = getTopBlock(window->playerGridId);
							if ( abs(getYPositionCenter(player) - getBlockYPositionCenter(testBlock)) < 4){
									window->playerGridId = getBlockId(testBlock);
							}
							break;
		
							case down: testBlock = getBottomBlock(window->playerGridId);
							if ( abs(getYPositionCenter(player) - getBlockYPositionCenter(testBlock)) < 4){
									window->playerGridId = getBlockId(testBlock);
							}
							break;
		
							case left: testBlock = getLeftBlock(window->playerGridId);
							if ( abs(getXPositionCenter(player) - getBlockXPositionCenter(testBlock)) < 4){
									window->playerGridId = getBlockId(testBlock);
							}
							break;	
		
							case right: testBlock = getRightBlock(window->playerGridId);
							if ( abs(getXPositionCenter(player) - getBlockXPositionCenter(testBlock)) < 4){
									window->playerGridId = getBlockId(testBlock);
							}
							break;
	
		case dead: return;
		
		default: return;
							
						}
}
				
							
/*~~~~~~~~~~~~~~~Updates Player Window: Based on Player Position, Traces Blocks surrounding Player~~~~~~~~~~~~~~~~~~~*/

void UpdatePlayerWindow(Window * window, Direction direction){
	int16_t playerXPosition = getXPositionCenter(window->tracedPlayer);
	int16_t playerYPosition = getYPositionCenter(window->tracedPlayer);
	
	switch(direction){
		case standing: return;
			             
		case up:    moveBlockIdUp(window, window->topBlock);
		            break;
		case down:  moveBlockIdDown(window, window->bottomBlock);	            
		            break;
		case left:  moveBlockIdLeft(window, window->leftBlock);   
		            break;
		case right: moveBlockIdRight(window, window->rightBlock);  
		            break;
		
		case dead:  return;
	}
	
}

/*~~~~~~~~~~~~~~~move block Id Up: Updates Window if Player moves into Top Block of previous Window~~~~~~~~~~~~~~~~~~~*/
void moveBlockIdUp(Window * window, Block * currentTopBlock){
	
	if (abs(getYPositionCenter(window->tracedPlayer) - getBlockYPositionCenter(currentTopBlock)) < 4){
		window->topBlock = getTopBlock(window->topBlockId);
		window->bottomBlock = getTopBlock(window->bottomBlockId);
		window->leftBlock = getTopBlock(window->leftBlockId);
		window->rightBlock = getTopBlock(window->rightBlockId);
		
		window->topBlockId = getBlockId(window->topBlock);
		window->bottomBlockId = getBlockId(window->bottomBlock);
		window->leftBlockId = getBlockId(window->leftBlock);
		window->rightBlockId = getBlockId(window->rightBlock);
		
	}
}

/*~~~~~~~~~~~~~~~move block Id Down: Updates Window if Player moves into Bottom Block of previous Window~~~~~~~~~~~~~~~~~~~*/
void moveBlockIdDown(Window * window, Block * currentBottomBlock){

	if (abs(getYPositionCenter(window->tracedPlayer) - getBlockYPositionCenter(currentBottomBlock)) < 4){
		window->topBlock = getBottomBlock(window->topBlockId);
		window->bottomBlock = getBottomBlock(window->bottomBlockId);
		window->leftBlock = getBottomBlock(window->leftBlockId);
		window->rightBlock = getBottomBlock(window->rightBlockId);
		
		window->topBlockId = getBlockId(window->topBlock);
		window->bottomBlockId = getBlockId(window->bottomBlock);
		window->leftBlockId = getBlockId(window->leftBlock);
		window->rightBlockId = getBlockId(window->rightBlock);
	}
}

/*~~~~~~~~~~~~~~~move block Id Left: Updates Window if Player moves into Left Block of previous Window~~~~~~~~~~~~~~~~~~~*/
void moveBlockIdLeft(Window * window, Block * currentLeftBlock){

	if (abs(getXPositionCenter(window->tracedPlayer) - getBlockXPositionCenter(currentLeftBlock)) < 4){
		window->topBlock = getLeftBlock(window->topBlockId);
		window->bottomBlock = getLeftBlock(window->bottomBlockId);
		window->leftBlock = getLeftBlock(window->leftBlockId);
		window->rightBlock = getLeftBlock(window->rightBlockId);
		
		window->topBlockId = getBlockId(window->topBlock);
		window->bottomBlockId = getBlockId(window->bottomBlock);
		window->leftBlockId = getBlockId(window->leftBlock);
		window->rightBlockId = getBlockId(window->rightBlock);
	}
}

/*~~~~~~~~~~~~~~~move block Id Right: Updates Window if Player moves into Right Block of previous Window~~~~~~~~~~~~~~~~~~~*/
void moveBlockIdRight(Window * window, Block * currentRightBlock){

	if (abs(getXPositionCenter(window->tracedPlayer) - getBlockXPositionCenter(currentRightBlock)) < 4){
		window->topBlock = getRightBlock(window->topBlockId);
		window->bottomBlock = getRightBlock(window->bottomBlockId);
		window->leftBlock = getRightBlock(window->leftBlockId);
		window->rightBlock = getRightBlock(window->rightBlockId);
		
		window->topBlockId = getBlockId(window->topBlock);
		window->bottomBlockId = getBlockId(window->bottomBlock);
		window->leftBlockId = getBlockId(window->leftBlock);
		window->rightBlockId = getBlockId(window->rightBlock);
	}
}
		
		
/*~~~~~~~~~~~~~~~Player Block Collision: Determines if Player in contact with Hard/Soft Block~~~~~~~~~~~~~~~~~~~*/
Hit Player_Block_Collision(Window * window){
	int16_t playerXCenter = getXPositionCenter(window->tracedPlayer);
	int16_t playerYCenter = getYPositionCenter(window->tracedPlayer);
	int16_t blockXCenter = getBlockXPositionCenter(window->topBlock);
	int16_t blockYCenter = getBlockYPositionCenter(window->topBlock);
	
	Player * player = window->tracedPlayer;
	Block * block = window->topBlock;
	
	if ((abs(getXPositionCenter(player) - getBlockXPositionCenter(block)) < 1) && getDirection(window->tracedPlayer) == up){
			if ((abs(getYPositionCenter(player) - getBlockYPositionCenter(block))) < 9 && (getBlockType(block->idNumber) == HardBlock || getBlockType(block->idNumber) == SoftBlock)){
			return contact;
		}
		else{
			return clear;
		}
	}
	

	blockXCenter = getBlockXPositionCenter(window->bottomBlock);
	blockYCenter = getBlockYPositionCenter(window->bottomBlock);
		
	block = window->bottomBlock;	
	if ((abs(getXPositionCenter(player) - getBlockXPositionCenter(block)) < 1) && getDirection(window->tracedPlayer) == down){
			if ((abs(getYPositionCenter(player) - getBlockYPositionCenter(block))) < 9 && (getBlockType(block->idNumber) == HardBlock || getBlockType(block->idNumber) == SoftBlock)){
			return contact;
		}
		else{
			return clear;
		}
	}
	

	blockXCenter = getBlockXPositionCenter(window->leftBlock);
	blockYCenter = getBlockYPositionCenter(window->leftBlock);

	block = window->leftBlock;	
	if ((abs(getYPositionCenter(player) - getBlockYPositionCenter(block)) < 1) && getDirection(window->tracedPlayer) == left){
			if ((abs(getXPositionCenter(player) - getBlockXPositionCenter(block))) < 9 && (getBlockType(block->idNumber) == HardBlock || getBlockType(block->idNumber) == SoftBlock)){
			return contact;
		}
		else{
			return clear;
		}
	}
	

	blockXCenter = getBlockXPositionCenter(window->rightBlock);
	blockYCenter = getBlockYPositionCenter(window->rightBlock);

	block = window->rightBlock;	
	if ((abs(getYPositionCenter(player) - getBlockYPositionCenter(block)) < 1) && getDirection(window->tracedPlayer) == right){
			if ((abs(getXPositionCenter(player) - getBlockXPositionCenter(block))) < 9 && (getBlockType(block->idNumber) == HardBlock || getBlockType(block->idNumber) == SoftBlock)){
			return contact;
		}
		else{
			return clear;
		}
	}
	return contact;
}

/*~~~~~~~~~~~~~~~Player Bomb Collision: Determines if Player in contact with Bomb~~~~~~~~~~~~~~~~~~~*/
Hit Player_Bomb_Collision(Window * window, Inventory * inventory1, Inventory * inventory2){
	
	Player * player = window->tracedPlayer;
	int16_t targetPositionId;
	Bombs * bomb;
	int16_t playerXCenter;
	int16_t playerYCenter;
	int16_t bombXCenter;
	int16_t bombYCenter;
	int16_t currentBombPlayer;
	
	targetPositionId = getWindowPlayerPositionId(window) - 15;
	currentBombPlayer = getBombPlayer(targetPositionId);
	
	if (getBombPlayer(targetPositionId) != -1){
		if (currentBombPlayer == 1){
			bomb = searchForBomb(inventory1, targetPositionId, getBombPlayer(targetPositionId));
		}
		if (currentBombPlayer == 2){
			bomb = searchForBomb(inventory2, targetPositionId, getBombPlayer(targetPositionId));
		}
	
		playerXCenter = getXPositionCenter(window->tracedPlayer);
		playerYCenter = getYPositionCenter(window->tracedPlayer);
		bombXCenter = getBombXCenter(bomb);
		bombYCenter = getBombYCenter(bomb);
		
		if ((abs(playerXCenter - bombXCenter) <1) && getDirection(player) == up){
				if ((abs(playerYCenter - bombYCenter) < 9 && getBombSetStatus(bomb) == set)){
					return contact;	
				}
				else{
					return clear;
			  }
	  }
	}
	

	targetPositionId = getWindowPlayerPositionId(window) + 15;
	currentBombPlayer = getBombPlayer(targetPositionId);
	
	if (getBombPlayer(targetPositionId) != -1){
		if (currentBombPlayer == 1){
			bomb = searchForBomb(inventory1, targetPositionId, getBombPlayer(targetPositionId));
		}
		if (currentBombPlayer == 2){
			bomb = searchForBomb(inventory2, targetPositionId, getBombPlayer(targetPositionId));
		}
	
		playerXCenter = getXPositionCenter(window->tracedPlayer);
		playerYCenter = getYPositionCenter(window->tracedPlayer);
		bombXCenter = getBombXCenter(bomb);
		bombYCenter = getBombYCenter(bomb);
		
		if ((abs(playerXCenter - bombXCenter) <1) && getDirection(player) == down){
				if ((abs(playerYCenter - bombYCenter) < 9 && getBombSetStatus(bomb) == set)){
					return contact;	
				}
				else{
					return clear;
			  }
	  }
	}
	
	targetPositionId = getWindowPlayerPositionId(window) - 1;
	currentBombPlayer = getBombPlayer(targetPositionId);
	
	if (getBombPlayer(targetPositionId) != -1){
		if (currentBombPlayer == 1){
			bomb = searchForBomb(inventory1, targetPositionId, getBombPlayer(targetPositionId));
		}
		if (currentBombPlayer == 2){
			bomb = searchForBomb(inventory2, targetPositionId, getBombPlayer(targetPositionId));
		}
	
		playerXCenter = getXPositionCenter(window->tracedPlayer);
		playerYCenter = getYPositionCenter(window->tracedPlayer);
		bombXCenter = getBombXCenter(bomb);
		bombYCenter = getBombYCenter(bomb);
		
		if ((abs(playerYCenter - bombYCenter) <1) && getDirection(player) == left){
				if ((abs(playerXCenter - bombXCenter) < 9 && getBombSetStatus(bomb) == set)){
					return contact;	
				}
				else{
					return clear;
			  }
	  }
	}
	
	targetPositionId = getWindowPlayerPositionId(window) + 1;
	currentBombPlayer = getBombPlayer(targetPositionId);
	
	if (getBombPlayer(targetPositionId) != -1){
		if (currentBombPlayer == 1){
			bomb = searchForBomb(inventory1, targetPositionId, getBombPlayer(targetPositionId));
		}
		if (currentBombPlayer == 2){
			bomb = searchForBomb(inventory2, targetPositionId, getBombPlayer(targetPositionId));
		}
	
		playerXCenter = getXPositionCenter(window->tracedPlayer);
		playerYCenter = getYPositionCenter(window->tracedPlayer);
		bombXCenter = getBombXCenter(bomb);
		bombYCenter = getBombYCenter(bomb);
		
		if ((abs(playerYCenter - bombYCenter) <1) && getDirection(player) == right){
				if ((abs(playerXCenter - bombXCenter) < 9 && getBombSetStatus(bomb) == set)){
					return contact;	
				}
				else{
					return clear;
			  }
	  }
	}
	return clear;
}

/*~~~~~~~~~~~~~~~Player PowerUp Collision: Determines if Player in contact with Power Up~~~~~~~~~~~~~~~~~~~*/
void Player_PowerUp_Collision(Window * window, Supply * supply){
	Player * player = window->tracedPlayer;
	int targetPositionId;
	PowerUp * powerUp;
	int16_t playerXCenter;
	int16_t playerYCenter;
	int16_t powerupXCenter;
	int16_t powerupYCenter;
	
	targetPositionId = getWindowPlayerPositionId(window) - 15;
	powerUp = getPowerUpFromListWithPosition(supply, targetPositionId);
	
	if (checkContainsPowerUp(supply, targetPositionId)!= -1 && getBlockHiddenStatus(targetPositionId) == uncovered){
		if (checkPowerUpStatus(getPowerUpFromListWithPosition(supply, targetPositionId)) == notUsed){
		playerXCenter = getXPositionCenter(player);
		playerYCenter = getYPositionCenter(player);
		powerupXCenter = getPowerUpXPositionCenter(powerUp);
		powerupYCenter = getPowerUpYPositionCenter(powerUp);
		
		if ((abs(playerXCenter - powerupXCenter) <1) && getDirection(player) == up){
				if (abs(playerYCenter - powerupYCenter) < 8 ){
					ActivatePowerUp(supply, player, targetPositionId);	
				}
			}
	  }
	}
	
	targetPositionId = getWindowPlayerPositionId(window) + 15;
	powerUp = getPowerUpFromListWithPosition(supply, targetPositionId);
	
	if (checkContainsPowerUp(supply, targetPositionId)!= -1 && getBlockHiddenStatus(targetPositionId) == uncovered){
		if (checkPowerUpStatus(getPowerUpFromListWithPosition(supply, targetPositionId)) == notUsed){
		
		playerXCenter = getXPositionCenter(player);
		playerYCenter = getYPositionCenter(player);
		powerupXCenter = getPowerUpXPositionCenter(powerUp);
		powerupYCenter = getPowerUpYPositionCenter(powerUp);
		
		if ((abs(playerXCenter - powerupXCenter) <1) && getDirection(player) == down){
				if (abs(playerYCenter - powerupYCenter) < 8 ){
					ActivatePowerUp(supply, player, targetPositionId);	
				}
			}
	  }
	}
	
	targetPositionId = getWindowPlayerPositionId(window) - 1;
	powerUp = getPowerUpFromListWithPosition(supply, targetPositionId);
	
	if (checkContainsPowerUp(supply, targetPositionId)!= -1 && getBlockHiddenStatus(targetPositionId) == uncovered){
		if (checkPowerUpStatus(getPowerUpFromListWithPosition(supply, targetPositionId)) == notUsed){
		
		playerXCenter = getXPositionCenter(player);
		playerYCenter = getYPositionCenter(player);
		powerupXCenter = getPowerUpXPositionCenter(powerUp);
		powerupYCenter = getPowerUpYPositionCenter(powerUp);
		
		if ((abs(playerYCenter - powerupYCenter) <1) && getDirection(player) == left){
				if (abs(playerXCenter - powerupXCenter) < 8 ){
					ActivatePowerUp(supply, player, targetPositionId);	
				}
			}
	  }
	}
	
	targetPositionId = getWindowPlayerPositionId(window) + 1;
	powerUp = getPowerUpFromListWithPosition(supply, targetPositionId);
	
	if (checkContainsPowerUp(supply, targetPositionId)!= -1 && getBlockHiddenStatus(targetPositionId) == uncovered){
		if (checkPowerUpStatus(getPowerUpFromListWithPosition(supply, targetPositionId)) == notUsed){
		
		playerXCenter = getXPositionCenter(player);
		playerYCenter = getYPositionCenter(player);
		powerupXCenter = getPowerUpXPositionCenter(powerUp);
		powerupYCenter = getPowerUpYPositionCenter(powerUp);
		
		if ((abs(playerYCenter - powerupYCenter) <1) && getDirection(player) == right){
				if (abs(playerXCenter - powerupXCenter) < 8 ){
					ActivatePowerUp(supply, player, targetPositionId);	
				}
			}
	  }
	}
}

int16_t getWindowPlayerPositionId(Window * window){
	return window->playerGridId;
}
