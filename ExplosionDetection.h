// ExplosionDetection.h
// Runs on LM4F120/TM4C123
// Bomb_RayTrace to determine which objects get destroyed during explosion
// Brian Tsai, Benson Huang
// Last Modified: 05/04/2017
#include <stdint.h>
#include "Struct.h"


void Bomb_RayTrace(Inventory * inventory, Player * player, Bombs * bomb, int16_t bombPositionId);
void DrawExplosion(Bombs * bomb, int16_t bombPositionId, Direction direction);
void UpdateBombExplosion(Player * currentPlayer1, Player * currentPlayer2,  Bombs * bomb);
void RemoveExplosion(Bombs * bomb, int16_t bombPositionId, Direction direction);
void determineStateOfLiving(Player * currentPlayer1, Player * currentPlayer2, int16_t positionId);
ExplosionStatus checkExplosionStatus(int16_t bombPositionId);
void Init_ObjectGrid(void);
void UpdatePlayerGrid(Player * player);
int16_t  checkBombGrid(int16_t bombPositionId);
int16_t  checkPlayerGrid(int16_t playerPositionId);
void UpdateBombGrid(Bombs * bomb);
void RemoveBombFromGrid(Bombs * bomb);
ExplosionStatus checkExplosionStatus(int16_t bombPositionId);
int16_t getExplosionXPosition(int16_t bombPositionId);
int16_t getExplosionYPosition(int16_t bombPositionId);
Range getExplosionRange(Player * player, int16_t explosionXPosition, int16_t explosionYPosition);
void UpdateBombStatusGrid(Bombs * bomb, ExplosionStatus explosionStatus);
int16_t getBombPlayer(int16_t bombPositionId);
int16_t  getPowerExplosionRange(Bombs * bomb);
void setPowerExplosionRange(Bombs * bomb, int16_t  newBombRange, Inventory * inventory);



