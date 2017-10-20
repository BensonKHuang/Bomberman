// CollisionDetection.h
// Runs on LM4F120/TM4C123
// Brian Tsai, Benson Huang
// Last Modified: 05/04/2017
#include <stdint.h>
#include "Struct.h"
//#include "Map.h"

void Init_Window(Window * window, Player * player, int16_t playerPositionId);
void UpdatePlayerId(Window * window, Direction direction);
void UpdatePlayerWindow(Window * window, Direction direction);


void moveBlockIdUp(Window * window, Block * currentBlock);
void moveBlockIdDown(Window * window, Block * currentBlock);
void moveBlockIdLeft(Window * window, Block * currentBlock);
void moveBlockIdRight(Window * window, Block * currentBlock);
void Player_PowerUp_Collision(Window * window, Supply * supply);

Hit Player_Block_Collision(Window * window);
Hit Player_Bomb_Collision(Window * window, Inventory * inventory1, Inventory * inventory2);
int16_t getWindowPlayerPositionId(Window * window);

extern Window window1;
extern Window window2;
