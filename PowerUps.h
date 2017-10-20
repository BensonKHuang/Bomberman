// PowerUps.h
// Runs on LM4F120/TM4C123
// Brian Tsai, Benson Huang
// Last Modified: 05/04/2017
#ifndef POWERUPS_HEADER
#define POWERUPS_HEADER
#include <stdint.h>
#include "Struct.h"

void PowerUps_Init(Supply * supply);
int RandomPowerUpNumber(void);	
PowerUpType getRandomPowerUp(void);
int RandomPowerUpPosition(void);

extern const unsigned short Power_Bomb[]; 
extern const unsigned short Power_Fire[];
extern const unsigned short Power_Skate[];


void Supply_Init(Supply * supply, int16_t newSupplySize);
void AddPowerUpToList(Supply * supply, PowerUpType newPowerUp, int16_t targetPowerUpPosition);
void RemovePowerUpFromListByIndex(Supply * supply, int16_t targetPowerUpIndex);
void RemovePowerUpFromListWithPosition(Supply * supply, int16_t targetPowerUpPositionId);
void RemovePowerUpFromListWithId(Supply * supply, int16_t targetPowerUpId);
PowerUp* getPowerUpFromListWithPosition(Supply * supply, int16_t targetPowerUpPositionId);
PowerUp* getPowerUpFromListWithId(Supply * supply, int16_t targetPowerUpId);
PowerUp* getPowerUpFromListWithIndex(Supply * supply, int16_t targetPowerUpIndex);



int16_t getPowerUpXPosition(PowerUp * powerUp);
int16_t getPowerUpYPosition(PowerUp * powerUp);
int16_t getPowerUpXPositionCenter(PowerUp * powerUp);
int16_t getPowerUpYPositionCenter(PowerUp * powerUp);
int16_t checkContainsPowerUp(Supply * supply, int16_t targetPowerUpPosition);
void setPowerUpPlayer(Player * player, PowerUp * powerUp);
void UpdatePowerUpTime(Supply * supply, Player * currentPlayer1, Player * currentPlayer2);
Effect checkPowerUpStatus(PowerUp * powerUp);


void ActivatePowerUp(Supply * supply, Player * player, int16_t targetPowerUpPosition);
void ActivatePowerBomb(Supply  * supply, Player * player, int16_t targetPowerUpPosition);
void ActivatePowerFire(Supply * supply, Player * player, int16_t targetPowerUpPosition);
void ActivatePowerSkate(Supply * supply, Player * player, int16_t targetPowerUpPosition);
void DeactivatePowerUp(Supply * supply, Player * player, PowerUpType currentPowerUp);

#endif
