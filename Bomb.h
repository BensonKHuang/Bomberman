// Bomb.h
// Runs on LM4F120/TM4C123
// Brian Tsai, Benson Huang
// Last Modified: 05/04/2017
#ifndef BOMB_HEADER
#define BOMB_HEADER

#include <stdint.h>
#include "Struct.h"


void Bomb_Init(Inventory * inventory, Player * player, Bombs * bomb, int16_t positionId, int16_t newBombId, int16_t newBombPlayer, int16_t numberofFrames);
void InsertBombFrame(Bombs * bomb, const unsigned short * frame);
void Draw_Bomb(Player * player, Bombs * bomb);
void Player_Snapshot(Player * player, Bombs * bomb, int16_t bombXPosition, int16_t bombYPosition, int16_t playerXPosition, int16_t playerYPosition);
void MapSnapshotGrid(Bombs * bomb);
void UpdateBomb(Player * player, Bombs * bomb);
const unsigned short* getBombFrame(Bombs * bomb);
Range getBombRange(Player * player, Bombs * bomb);
void UpdateBombAnimation(Player * player, Bombs * bomb);
void Init_BombGrid(void);
void UpdateBombGrid(Bombs * bomb);
void RemoveBombFromGrid(Bombs * bomb);
Bombs* getBomb(int16_t bombPositionId);
int16_t getBombPositionId(Bombs * bomb);
int16_t getBombId(Bombs * bomb);
int16_t getExplosionXPosition(int16_t bombPositionId);
int16_t getExplosionYPosition(int16_t bombPositionId);
int16_t getBombXCenter(Bombs * bomb);
int16_t getBombYCenter(Bombs * bomb);
BombSet getBombSetStatus(Bombs * bomb);
CharacterType getCharacterType(Bombs * bomb, Direction direction, int currentRange);
void setBombRange(Bombs * bomb, int newBombRange);
extern const unsigned short Fire3[]; 
extern const unsigned short Bomb[];
#endif




