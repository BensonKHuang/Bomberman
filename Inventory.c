// Inventory.c
// Runs on LM4F120/TM4C123
// Brian Tsai, Benson Huang
// Last Modified: 05/04/2017

#define oneSecondUpper 81333333;
#define oneSecondLower 78666666;
#include <stdint.h>
#include <stdlib.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
#include "Inventory.h"
#include "Bomb.h"
#include "Player.h"
#include "ExplosionDetection.h"
#include "PowerUps.h"
#include "Map.h"


Bombs * mainBomb;
	/*~~~~~~~~~~~~~~~~~~~~~~Initializes Inventory~~~~~~~~~~~~~~~~~~~*/
void Inventory_Init(Inventory * inventory){
	inventory->bagStatus = empty;
	inventory->inventorySize = 4;
	//inventory->newBombIndex = 0;
	inventory->detonationIndex = 0;
	inventory->putBombIndex = 0;
	inventory->getBombIndex = 0;
}

InventoryStatus checkInventoryStatus(Inventory * inventory){
	return inventory->bagStatus;
}

void setInventorySize(Inventory * inventory, int16_t newSize){
	inventory->inventorySize = newSize;
}
	
	/*~~~~~~~~~~~~~~~~~~~~~~add Bomb to List ~~~~~~~~~~~~~~~~~~~*/
void addBombToList(Inventory * inventory, Player * player, int16_t bombPositionId){
	
	if (inventory->putBombIndex == inventory->inventorySize - 1){
		inventory->bagStatus = full;
		return;
	}
	else{
		if (checkBombGrid(bombPositionId) == -1){
		//placeBomb(&player1, &(inventory->inventoryBag[inventory->putBombIndex]), inventory->insertBombIndex, bombPositionId, 6);
		Bomb_Init(inventory, player, &(inventory->inventoryBag[inventory->putBombIndex]), bombPositionId, inventory->putBombIndex, player->playerId, 6);
		//UpdateBombGrid(&inventory->inventoryBag[inventory->powerDetonationIndex]);
		inventory->detonationIndex = inventory->putBombIndex;
		inventory->putBombIndex ++;
		inventory->bagStatus = notEmpty;
		}
		else{
			return;
		}
		
	}
}
		/*~~~~~~~~~~~~~~~~~~~~~~remove Bomb from List~~~~~~~~~~~~~~~~~~~*/
void removeBombFromList(Inventory * inventory, int16_t removeBombIndex){
	
	int16_t currentBombIndex;
	for (currentBombIndex = removeBombIndex; currentBombIndex < inventory->putBombIndex; currentBombIndex++ ){
		inventory->inventoryBag[currentBombIndex] = inventory->inventoryBag[currentBombIndex + 1];
	}
	
	inventory->putBombIndex --;
	inventory->detonationIndex = inventory->putBombIndex;
	if (inventory->getBombIndex == inventory->putBombIndex){
		inventory->bagStatus = empty;
	}
	else{
		inventory->bagStatus = notEmpty;
	}
	 
		
}
		/*~~~~~~~~~~~~~~~~~~~~~~remove Bomb from list with ID ~~~~~~~~~~~~~~~~~~~*/
void removeBombFromListWithId(Inventory * inventory, int16_t targetBombId){
	int16_t currentBombIndex;
	int16_t removeBombIndex; 
	for (currentBombIndex = 0; currentBombIndex < inventory->putBombIndex; currentBombIndex ++){
		if (inventory->inventoryBag[currentBombIndex].bombId == targetBombId){
			inventory->inventoryBag[currentBombIndex].bombId = -1;
			removeBombIndex = currentBombIndex;
			for (removeBombIndex = currentBombIndex; removeBombIndex < inventory->putBombIndex; removeBombIndex ++){
				inventory->inventoryBag[removeBombIndex] = inventory->inventoryBag[removeBombIndex + 1];
			}
			break;
		}
	}
	inventory->putBombIndex --;
	inventory->detonationIndex = inventory->putBombIndex;
	if (inventory->getBombIndex == inventory->putBombIndex){
		inventory->bagStatus = empty;
	}
	else{
		inventory->bagStatus = notEmpty;
	}
}
	
/*~~~~~~~~~~~~~~~~~~~~~~Get Newest bomb from list: ensures you can put maximum number of bombs at a time~~~~~~~~~~~~~~~~~~~*/
Bombs* getNewestBombFromList(Inventory * inventory){
	return &(inventory->inventoryBag[inventory->detonationIndex]);
}
	/*~~~~~~~~~~~~~~~~~~~~~~Get Bomb from List with Position, for perimeter checks ~~~~~~~~~~~~~~~~~~~*/
Bombs* getBombFromListWithPosition(Inventory * inventory, int16_t targetBombPositionId){
	//return &(inventory->inventoryBag[targetIndex]);
	int16_t bombIndex;
	for (bombIndex = inventory->getBombIndex; bombIndex < inventory->putBombIndex; bombIndex++){
		if (inventory->inventoryBag[bombIndex].positionId == targetBombPositionId){
			break;
		}
	}
	return &inventory->inventoryBag[bombIndex];
}

Bombs* getBombFromListWithIndex(Inventory * inventory, int16_t targetIndex){
	return &(inventory->inventoryBag[targetIndex]);
}

/*~~~~~~~~~~~~~~~~~~~~~~Update Count Down for Bomb explosion: Approximately 2 seconds (40 counts)~~~~~~~~~~~~~~~~~~~*/
void UpdateCountDownTime(Inventory * inventory, Player * player){
	int16_t bombUpdateIndex;
	Bombs * bomb;
	for (bombUpdateIndex = inventory->getBombIndex; bombUpdateIndex < inventory->putBombIndex; bombUpdateIndex ++){
		bomb = &inventory->inventoryBag[bombUpdateIndex];
		if (bomb->bombId != -1){
			bomb->detonationTime --;
			if (bomb->hasExploded == notExploded){
				if (bomb->detonationTime == 0){
					Bomb_RayTrace(inventory, player, bomb, bomb->positionId);
					}
			}
		}
	}
}
/*~~~~~~~~~~~~~~~~~~~~~~Update Ticking Bomb: Changes the sprite of the bomb after counter reaches ~~~~~~~~~~~~~~~~~~~*/
void UpdateTickingBomb(Inventory * inventory, Player * player){
	int16_t bombUpdateIndex;
	Bombs * bomb;
	if (checkInventoryStatus(inventory) != empty){
		for (bombUpdateIndex = inventory->getBombIndex; bombUpdateIndex < inventory->putBombIndex; bombUpdateIndex ++){
			bomb = &inventory->inventoryBag[bombUpdateIndex];
			if (getBombRange(player, getBombFromListWithIndex(inventory, bombUpdateIndex)) == inRange && bomb->hasExploded == notExploded){
					UpdateBomb(player, getBombFromListWithIndex(inventory, bombUpdateIndex));
				}
			else if (bomb->hasExploded == notExploded){
					UpdateBombAnimation(player, getBombFromListWithIndex(inventory, bombUpdateIndex));
				}
			}
		}
	else {
			return;
	} 
}
/*~~~~~~~~~~~~~~~~~~~~~~Update Explosion: Checks for when bomb == exploding and uncovers power ups~~~~~~~~~~~~~~~~~~~*/
void UpdateExplosion(Inventory * inventory1, Inventory * inventory2, Player * currentPlayer1, Player * currentPlayer2){
	int16_t bombUpdateIndex;
	Bombs * bomb;
	
		
	for (bombUpdateIndex = inventory1->getBombIndex; bombUpdateIndex < inventory1->putBombIndex; bombUpdateIndex ++){
		bomb = &(inventory1->inventoryBag[bombUpdateIndex]);
		if (bomb->hasExploded == exploding){
			UpdateBombExplosion(currentPlayer1, currentPlayer2, bomb);
		}
	}
	
	for (bombUpdateIndex = inventory2->getBombIndex; bombUpdateIndex < inventory2->putBombIndex; bombUpdateIndex ++){
		bomb = &(inventory2->inventoryBag[bombUpdateIndex]);
		if (bomb->hasExploded == exploding){
			UpdateBombExplosion(currentPlayer1, currentPlayer2, bomb);
		}
	}
	
	for (bombUpdateIndex = inventory1->getBombIndex; bombUpdateIndex < inventory1->putBombIndex; bombUpdateIndex ++){
		bomb = &inventory1->inventoryBag[bombUpdateIndex];
		if (bomb->hasExploded == exploded){
			UpdateBombExplosion(currentPlayer1, currentPlayer2, bomb);
			UpdateBombStatusGrid(bomb, notExploded);
			if (bomb->up == Block_Object && containsPowerUpByBlock(getBlock(bomb->positionId - 15)) != NoPowerUp){
			UncoverPowerUp(getBlock(bomb->positionId - 15));
			setBlockHiddenStatus(uncovered, bomb->positionId - 15);
			}
			if (bomb->down == Block_Object && containsPowerUpByBlock(getBlock(bomb->positionId + 15)) != NoPowerUp){
			UncoverPowerUp(getBlock(bomb->positionId + 15));
			setBlockHiddenStatus(uncovered, bomb->positionId - 15);

			}
			if (bomb->left == Block_Object && containsPowerUpByBlock(getBlock(bomb->positionId - 1)) != NoPowerUp){
			UncoverPowerUp(getBlock(bomb->positionId - 1));
			setBlockHiddenStatus(uncovered, bomb->positionId - 15);
	
			}
			if (bomb->right == Block_Object && containsPowerUpByBlock(getBlock(bomb->positionId + 1)) != NoPowerUp){
			UncoverPowerUp(getBlock(bomb->positionId + 1));
			setBlockHiddenStatus(uncovered, bomb->positionId - 15);
	
		} 
			
			RemoveBombFromGrid(bomb);
			removeBombFromListWithId(inventory1, bomb->bombId);
		}
	}
	
	for (bombUpdateIndex = inventory2->getBombIndex; bombUpdateIndex < inventory2->putBombIndex; bombUpdateIndex ++){
		bomb = &inventory2->inventoryBag[bombUpdateIndex];
		if (bomb->hasExploded == exploded){
			UpdateBombExplosion(currentPlayer1, currentPlayer2, bomb);
			UpdateBombStatusGrid(bomb, notExploded);
			RemoveBombFromGrid(bomb);
			removeBombFromListWithId(inventory2, bomb->bombId);
		}
	}
}
/*~~~~~~~~~~~~~~~~~~~~~~Search for Bomb: returns bomb ID from bag~~~~~~~~~~~~~~~~~~~*/
Bombs* searchForBomb(Inventory * inventory, int16_t bombPositionId, int16_t currentBombPlayer){
	int16_t bombSearchIndex;
	for (bombSearchIndex = inventory->getBombIndex; bombSearchIndex < inventory->putBombIndex; bombSearchIndex ++){
		if (inventory->inventoryBag[bombSearchIndex].positionId == bombPositionId){
			break;
			}
	}
	return &inventory->inventoryBag[bombSearchIndex];
}
