// PowerUps.c
// Runs on LM4F120/TM4C123
// Brian Tsai, Benson Huang
// Last Modified: 05/04/2017
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include <stdlib.h>
#include "ST7735.h"
#include "Random.h"
#include "Map.h"
#include "PowerUps.h"
#include "Player.h"
#include "Inventory.h"
#include "ExplosionDetection.h"
#include "Music.h"
#include "Sound.h"
#include "Timer1.h"

/*~~~~~~~~~~~~~~~~~~~~~~Initializes PowerUps: Uses RNG to place powerups randomly accross map~~~~~~~~~~~~~~~~~~~*/
void PowerUps_Init(Supply * supply){
	int16_t PowerUps = 10;
	int16_t row;
	int16_t col;
	Supply_Init(supply, PowerUps);
	while (PowerUps != 0){
		row = RandomPowerUpPosition();
		col = RandomPowerUpPosition();
		if (getBlockType(row * 15 + col) == GrassBlock || getBlockType(row * 15 + col) == SoftBlock){
			if (containsPowerUpByPositionId(row * 15 + col) == NoPowerUp){
				PowerUpType newPowerUpType = getRandomPowerUp();
				setPowerUp(newPowerUpType, row * 15 + col);
				AddPowerUpToList(supply, newPowerUpType, row * 15 + col);
				PowerUps -- ;
			}
		}
	}
}
	//Initializes Supply Struct to 0
void Supply_Init(Supply * supply, int16_t newSupplySize){
	supply->supplyStatus = empty;
	supply->supplySize = newSupplySize;
	supply->getPowerUpIndex = 0;
	supply->putPowerUpIndex = 0;
	supply->newPowerUpIndex = 0;
}


	//Adds power up to list, called by PowerUps_Init() 
void AddPowerUpToList(Supply * supply, PowerUpType newPowerUp, int16_t targetPowerUpPosition){
	if (supply->putPowerUpIndex == supply->supplySize - 1){
		supply->supplyStatus = full;
	}
	else {
		supply->supplyBag[supply->putPowerUpIndex].xPosition = getBlockXPositionById(targetPowerUpPosition);
		supply->supplyBag[supply->putPowerUpIndex].yPosition = getBlockYPositionById(targetPowerUpPosition);
		supply->supplyBag[supply->putPowerUpIndex].powerUp = newPowerUp;
		supply->supplyBag[supply->putPowerUpIndex].effectTime = 600;
		supply->supplyBag[supply->putPowerUpIndex].powerUpPositionId = targetPowerUpPosition;
		supply->supplyBag[supply->putPowerUpIndex].powerUpStatus = notUsed;
		supply->newPowerUpIndex = supply->putPowerUpIndex;
		supply->supplyBag[supply->putPowerUpIndex].powerUpId = supply->putPowerUpIndex;
		supply->putPowerUpIndex ++;
		
		supply->supplyBag[supply->putPowerUpIndex].powerUpPlayer = -1;
		supply->supplyStatus = notEmpty;
	}
}
	//Removes PowerUp from List after it has been activated
void RemovePowerUpFromListByIndex(Supply * supply, int16_t targetPowerUpIndex){
	int16_t powerupRemoveIndex;
	for (powerupRemoveIndex = targetPowerUpIndex; powerupRemoveIndex < supply->putPowerUpIndex; powerupRemoveIndex ++){
		supply->supplyBag[powerupRemoveIndex] = supply->supplyBag[powerupRemoveIndex + 1];
	}
	
	supply->putPowerUpIndex --;
	supply->newPowerUpIndex = supply->putPowerUpIndex;
	if (supply->getPowerUpIndex == supply->putPowerUpIndex){
		supply->supplyStatus = empty;
	}
	else{
		supply->supplyStatus = notEmpty;
	}
}
/*~~~~~~~~~~~~~~~~~~~~~~Removes Power Up from List: When Bomb or Player collision occurs with PowerUp~~~~~~~~~~~~~~~~~~~*/
void RemovePowerUpFromListWithPosition(Supply * supply, int16_t targetPowerUpPositionId){
	int16_t currentPowerUpIndex;
	int16_t removePowerUpIndex;
	
	for (currentPowerUpIndex = supply->getPowerUpIndex; currentPowerUpIndex < supply->putPowerUpIndex; currentPowerUpIndex ++){
		if (supply->supplyBag[currentPowerUpIndex].powerUpPositionId == targetPowerUpPositionId){
			supply->supplyBag[currentPowerUpIndex].powerUpId = -1;
			for (removePowerUpIndex = currentPowerUpIndex; removePowerUpIndex < supply->putPowerUpIndex; removePowerUpIndex ++){
				supply->supplyBag[removePowerUpIndex] = supply->supplyBag[removePowerUpIndex + 1];
			}
			break;
		}
	}
	supply->putPowerUpIndex --;
	supply->newPowerUpIndex = supply->putPowerUpIndex;
	if (supply->getPowerUpIndex == supply->putPowerUpIndex){
		supply->supplyStatus = empty;
	}
	else{
		supply->supplyStatus = notEmpty;
	}
}
/*~~~~~~~~~~~~~~~~~~~~~~Removes PowerUp from List with ID~~~~~~~~~~~~~~~~~~*/
void RemovePowerUpFromListWithId(Supply * supply, int16_t targetPowerUpId){
	int16_t currentPowerUpIndex;
	int16_t removePowerUpIndex;
	
	for (currentPowerUpIndex = supply->getPowerUpIndex; currentPowerUpIndex < supply->putPowerUpIndex; currentPowerUpIndex ++){
		if (supply->supplyBag[currentPowerUpIndex].powerUpId == targetPowerUpId){
			supply->supplyBag[currentPowerUpIndex].powerUpId = -1;
			for (removePowerUpIndex = currentPowerUpIndex; removePowerUpIndex < supply->putPowerUpIndex; removePowerUpIndex ++){
				supply->supplyBag[removePowerUpIndex] = supply->supplyBag[removePowerUpIndex + 1];
			}
			break;
		}
	}
	supply->putPowerUpIndex --;
	supply->newPowerUpIndex = supply->putPowerUpIndex;
	if (supply->getPowerUpIndex == supply->putPowerUpIndex){
		supply->supplyStatus = empty;
	}
	else{
		supply->supplyStatus = notEmpty;
	}
}
	/*~~~~~~~~~~~~~~~~~~~~~~Get PowerUp From List with Position: Checks position ID with Supply to find index in Power Array~~~~~~~~~~~~~~~~~~~*/
PowerUp* getPowerUpFromListWithPosition(Supply * supply, int16_t targetPowerUpPositionId){
	int16_t powerUpIndex;
	for (powerUpIndex = supply->getPowerUpIndex; powerUpIndex < supply->putPowerUpIndex; powerUpIndex++){
		if (supply->supplyBag[powerUpIndex].powerUpPositionId == targetPowerUpPositionId){
			break;
		}
	}
	return &(supply->supplyBag[powerUpIndex]);
}
/*~~~~~~~~~~~~~~~~~~~~~~Get PowerUp From List with Position: Checks position ID with Supply to find index in Power Array~~~~~~~~~~~~~~~~~~~*/
PowerUp* getPowerUpFromListWithId(Supply * supply, int16_t targetPowerUpId){
	int16_t powerUpIndex;
	for (powerUpIndex = supply->getPowerUpIndex; powerUpIndex < supply->putPowerUpIndex; powerUpIndex++){
		if (supply->supplyBag[powerUpIndex].powerUpPositionId == targetPowerUpId){
			break;
		}
	}
	return &supply->supplyBag[powerUpIndex];
}
/*~~~~~~~~~~~~~~~~~~~~~~Get PowerUp from List with ID: Checks ID with Supply to find positionID~~~~~~~~~~~~~~~~~~~*/
PowerUp* getPowerUpFromListWithIndex(Supply * supply, int16_t targetPowerUpIndex){
	return &supply->supplyBag[targetPowerUpIndex];
}


int RandomPowerUpNumber(void){
	//Random_Init(NVIC_ST_CURRENT_R);
	return((Random32()>>24)%6) + 5;
}
int RandomPowerUpPosition(void){
	return ((Random32()>>24)%11) + 2;
}

PowerUpType getRandomPowerUp(void){
	int randomPowerUp = ((Random32()>>24)%3);
	PowerUpType randomPowerUpType;
	switch (randomPowerUp){
		case 0: randomPowerUpType = PowerBomb; break;
		case 1: randomPowerUpType = PowerSkate; break;
		case 2: randomPowerUpType = PowerFire; break;
	}
	return randomPowerUpType;
}
/*~~~~~~~~~~~~~~~~~~~~~~get Power up Range: Checks PowerUp Proximity for collision with Player ~~~~~~~~~~~~~~~~~~~*/
Range getPowerUpRange(Player * player, PowerUp * powerUp){
	int16_t overlapX = abs(player->xPosition - powerUp->xPosition);
	int16_t overlapY = abs(player->yPosition - powerUp->yPosition);
	
	if (overlapX < 8 && overlapY < 8){
		return inRange;
	}
	else{
		return outOfRange;
	}
}

int16_t getPowerUpXPosition(PowerUp * powerUp){
	return powerUp->xPosition;
}


int16_t getPowerUpYPosition(PowerUp * powerUp){
	return powerUp->yPosition;
}

int16_t getPowerUpXPositionCenter(PowerUp * powerUp){
	return powerUp->xPosition + 4;
}

int16_t getPowerUpYPositionCenter(PowerUp * powerUp){
	return powerUp->yPosition - 4;
}

void setPowerUpPlayer(Player * player, PowerUp * powerUp){
	powerUp->powerUpPlayer = player->playerId;
}
int16_t getPowerUpPlayer(PowerUp * powerUp){
	return powerUp->powerUpPlayer;
}

int16_t checkContainsPowerUp(Supply * supply, int16_t targetPowerUpPosition){
	int16_t checkPowerUpIndex;
	for (checkPowerUpIndex = supply->getPowerUpIndex; checkPowerUpIndex < supply->putPowerUpIndex; checkPowerUpIndex ++){
		if (supply->supplyBag[checkPowerUpIndex].powerUpPositionId == targetPowerUpPosition)
			return supply->supplyBag[checkPowerUpIndex].powerUpId;
	}
	return -1;
}



Effect checkPowerUpStatus(PowerUp * powerUp){
	return powerUp->powerUpStatus;
}
/*~~~~~~~~~~~~~~~~~~~~~~Update Power Time: Decremets counter until Power Duration is finished~~~~~~~~~~~~~~~~~~~*/
void UpdatePowerUpTime(Supply * supply, Player * currentPlayer1, Player * currentPlayer2){
	int16_t updatePowerUpIndex;
	for (updatePowerUpIndex = supply->getPowerUpIndex; updatePowerUpIndex < supply->putPowerUpIndex; updatePowerUpIndex ++){
			if (supply->supplyBag[updatePowerUpIndex].powerUpStatus == inUse){
				supply->supplyBag[updatePowerUpIndex].effectTime --;
				if (currentPlayer1->soundTimer != 0){
					currentPlayer1->soundTimer--;
				}
				else if (currentPlayer2->soundTimer != 0){
					currentPlayer2->soundTimer--;
				}
				else{
					Timer1A_Stop();
					Timer3A_Stop();
				}
			}
		}
	
		for (updatePowerUpIndex = supply->getPowerUpIndex; updatePowerUpIndex < supply->putPowerUpIndex; updatePowerUpIndex ++){
			if (supply->supplyBag[updatePowerUpIndex].effectTime < 1){
				int16_t currentPowerUpPlayer = getPowerUpPlayer(&supply->supplyBag[updatePowerUpIndex]);
				if ( currentPowerUpPlayer == currentPlayer1->playerId){
					DeactivatePowerUp(supply, currentPlayer1, supply->supplyBag[currentPowerUpPlayer].powerUp); 
					supply->supplyBag[updatePowerUpIndex].powerUpStatus = used;
				}
				else {
					DeactivatePowerUp(supply, currentPlayer2, supply->supplyBag[currentPowerUpPlayer].powerUp);
					supply->supplyBag[updatePowerUpIndex].powerUpStatus = used;
				}
			}
		}
	
		for (updatePowerUpIndex = supply->getPowerUpIndex; updatePowerUpIndex < supply->putPowerUpIndex; updatePowerUpIndex++){
			if (supply->supplyBag[updatePowerUpIndex].powerUpStatus == used){
				RemovePowerUpFromListByIndex(supply, updatePowerUpIndex);
			}
		}
	
}
/*~~~~~~~~~~~~~~~~~~~~~~Activates PowerUp: Depending on powerup, calls Bomb, Fire, or Skate~~~~~~~~~~~~~~~~~~~*/
void ActivatePowerUp(Supply * supply, Player * player, int16_t targetPowerUpPosition){
	Sound_Play(4);
	Timer1A_Start();
	Timer3A_Start();
	
	
	PowerUp * powerUp = getPowerUpFromListWithPosition(supply, targetPowerUpPosition);
	PowerUpType powerUpType = containsPowerUpByPositionId(targetPowerUpPosition);
	if ( player->playerPowerUp != NoPowerUp){
		DeactivatePowerUp(supply, player, player->playerPowerUp);
	}
	
	switch (powerUpType){
		case NoPowerUp: break;
		case PowerBomb: ActivatePowerBomb(supply, player, targetPowerUpPosition); break;
		case PowerFire: ActivatePowerFire(supply, player, targetPowerUpPosition); break;
		case PowerSkate: ActivatePowerSkate(supply, player, targetPowerUpPosition); break;
	}
		setPowerUpPlayer(player, powerUp); 
		player->soundTimer = 5;
}
					/*~~~~~~~~~~~~~~~~~~~~~~Activate Power Bomb, increases inventory array size from 4 to 7 (3 to 6 bombs can be placed)~~~~~~~~~~~~~~~~~~~*/
void ActivatePowerBomb(Supply * supply, Player * player, int16_t targetPowerUpPosition){
	int16_t powerUpXPosition = getBlockXPositionById(targetPowerUpPosition);
	int16_t powerUpYPosition = getBlockYPositionById(targetPowerUpPosition);
	player->playerPowerUp = PowerBomb;
	PowerUp * powerUp = getPowerUpFromListWithPosition(supply, targetPowerUpPosition);
	powerUp->powerUpStatus= inUse;
	player->currentPowerUp = powerUp;
	ST7735_DrawBitmap(powerUpXPosition, powerUpYPosition, Grass_block, 8 , 8);
	if (player->playerId == 1){
		setInventorySize(&inventory1, 7);
	}
	else{
		setInventorySize(&inventory2, 7);
	}
}

/*~~~~~~~~~~~~~~~~~~~~~~ Activate Power Fire: Raises flag "PowerFire" to unlock specific conditionals with bomb trace... Adds additional tile of explosion ~~~~~~~~~~~~~~~~~~~*/
void ActivatePowerFire(Supply * supply, Player * player, int16_t targetPowerUpPosition){
	int16_t powerUpXPosition = getBlockXPositionById(targetPowerUpPosition);
	int16_t powerUpYPosition = getBlockYPositionById(targetPowerUpPosition);
	player->playerPowerUp = PowerFire;
	PowerUp * powerUp = getPowerUpFromListWithPosition(supply, targetPowerUpPosition);
	powerUp->powerUpStatus= inUse;
	player->currentPowerUp = powerUp;
	ST7735_DrawBitmap(powerUpXPosition, powerUpYPosition, Grass_block, 8 , 8);
}
/*~~~~~~~~~~~~~~~~~~~~~~Activate Power Skate: Increases multipliers to 2 ~~~~~~~~~~~~~~~~~~~*/
void ActivatePowerSkate(Supply * supply, Player * player, int16_t targetPowerUpPosition){
	int16_t  powerUpXPosition = getBlockXPositionById(targetPowerUpPosition);
	int16_t  powerUpYPosition = getBlockYPositionById(targetPowerUpPosition);
	player->playerPowerUp = PowerSkate;
	PowerUp * powerUp = getPowerUpFromListWithPosition(supply, targetPowerUpPosition);
	powerUp->powerUpStatus= inUse;
	setHorizontalVelocityMultiplier(player, 2);
	setVerticalVelocityMultiplier(player, 2);
	player->currentPowerUp = powerUp;
	ST7735_DrawBitmap(powerUpXPosition, powerUpYPosition, Grass_block, 8 , 8);
}
/*~~~~~~~~~~~~~~~~~~~~~~Deactivates PowerUp: After 30 seconds has passed or picking up another item~~~~~~~~~~~~~~~~~~~*/
void DeactivatePowerUp(Supply * supply, Player * player, PowerUpType currentPowerUp){
	switch(currentPowerUp){
		case NoPowerUp: break;
		case PowerFire: 
			              RemovePowerUpFromListWithId(supply, player->currentPowerUp->powerUpId); 
		                break;
		case PowerSkate: setHorizontalVelocityMultiplier(player, 1);
						         setVerticalVelocityMultiplier(player, 1);
		                 RemovePowerUpFromListWithId(supply, player->currentPowerUp->powerUpId); 
		                 break;
		case PowerBomb: if(player->playerId == 1){
				              setInventorySize(&inventory1, 4); 
		                  RemovePowerUpFromListWithId(supply, player->currentPowerUp->powerUpId); 
		                  break; }
		                else {
	                    setInventorySize(&inventory2, 4); 
										  RemovePowerUpFromListWithId(supply, player->currentPowerUp->powerUpId); 
										  break; }
										}
	player->playerPowerUp = NoPowerUp;
			
}

	
