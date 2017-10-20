// Inventory.h
// Runs on LM4F120/TM4C123
// Brian Tsai, Benson Huang
// Last Modified: 05/04/2017
#include <stdint.h>
#include "Struct.h"

void Inventory_Init(Inventory * inventory);
InventoryStatus checkInventoryStatus(Inventory * inventory);
void setInventorySize(Inventory * inventory, int16_t newSize);

void addBombToList(Inventory * inventory, Player * player, int16_t bombPositionId);
void removeBombFromList(Inventory * inventory, int16_t removeBombIndex);
Bombs* getBombFromListWithPosition(Inventory * inventory, int16_t targetBombPositionId);
Bombs* getBombFromListWithIndex(Inventory * inventory, int16_t targetIndex);
void UpdateCountDownTime(Inventory * inventory, Player * player);
void removeBombFromListWithId(Inventory * inventory, int16_t targetBombId);
void UpdateTickingBomb(Inventory * inventory, Player * player);
Bombs* getNewestBombFromList(Inventory * inventory);
void UpdateExplosion(Inventory * inventory1, Inventory * inventory2, Player * currentPlayer1, Player * currentPlayer2);
Bombs* searchForBomb(Inventory * inventory, int16_t bombPositionId, int16_t currentBombPlayer);


void PowerInventory_Init(Inventory * inventory);
InventoryStatus powerCheckInventoryStatus(Inventory * inventory);
void powerAddBombToList(Inventory * inventory, Player * player, int16_t bombPositionId);
void powerRemoveBombFromList(Inventory * inventory, int16_t removeBombIndex);
Bombs* powerGetBombFromListWithPosition(Inventory * inventory, int16_t targetBombPositionId);
Bombs* powerGetBombFromListWithIndex(Inventory * inventory, int16_t targetIndex);
void PowerUpdateCountDownTime(Inventory * inventory, Player * player);
void powerRemoveBombFromListWithId(Inventory * inventory, int16_t targetBombId);
void PowerUpdateTickingBomb(Inventory * inventory, Player * player);
void PowerUpdateExplosion(Inventory * inventory, Player * currentPlayer1, Player * currentPlayer2);
Bombs* powerSearchForBomb(Inventory * inventory, int16_t bombPositionId, int16_t currentBombPlayer);
Bombs* powerGetNewestBombFromList(Inventory * inventory);


extern Inventory inventory1;
extern Inventory inventory2;
