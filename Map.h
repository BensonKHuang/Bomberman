// Map.h
// Runs on LM4F120/TM4C123
// Brian Tsai, Benson Huang
// Last Modified: 05/04/2017
#ifndef MAP_HEADER
#define MAP_HEADER

#include <stdint.h>
#include "Struct.h"

void Map_Init(Supply * supply);
void Perimeter(void);
void InnerHardBlocks(void);
void Grass(void);
void RandomizeCorners(void);
void DrawGrid(void);
void SoftBlockRandomizer(void);
int RandomGridPosition(void);
int RandomSoftBlocks(void);
int RandomCornerOrientation(void);
int16_t getBlockXPosition(Block * block);
int16_t getBlockYPosition(Block * block);
int16_t getBlockXPositionCenter(Block * block);
int16_t getBlockYPositionCenter(Block * block);
void IdBlocks(void);
Block* getBlock(int16_t positionId);
Block* getTopBlock(int16_t playerPositionId);
Block* getBottomBlock(int16_t playerPositionId);
Block* getLeftBlock(int16_t playerPositionId);
Block* getRightBlock(int16_t playerPositionId);
int16_t getBlockId(Block * block);
Type getBlockType(int16_t targetPositionId);
int16_t getBlockXPositionById(int16_t targetPositionId);
int16_t getBlockYPositionById(int16_t targetPositionId);
void UpdateGridBlock(Block * block);
Destructible destructibleStatus (int16_t positionId);
void setPowerUp(PowerUpType powerUp, int16_t positionId);
void setBlockType(Type newBlockType, int16_t positionId);
PowerUpType containsPowerUpByPositionId(int16_t targetPowerUpPosition);
PowerUpType containsPowerUpByBlock(Block * block);
void UncoverPowerUp(Block * block);
void GenerateNewPerimeter(Player * currentPlayer1, Player * currentPlayer2);
Hidden getBlockHiddenStatus(int positionId);
void setBlockHiddenStatus(Hidden newHiddenStatus, int positionId);
void ClearGrid(void);
void NewPerimeter(void);
extern const unsigned short Solid_block[];
extern const unsigned short Brick_block[];
extern const unsigned short Grass_block[];

#endif

