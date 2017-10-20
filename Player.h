// Player.h
// Runs on LM4F120/TM4C123
// Brian Tsai, Benson Huang
// Last Modified: 05/04/2017
#ifndef PLAYER_HEADER
#define PLAYER_HEADER
#include <stdint.h>
#include "Struct.h"

void Player_Init(Player * newPlayer, int16_t startingX, int16_t startingY, int16_t idNumber);
void Movement_Init(Player * player);
void ConfigureStandingSprite(Player * player);
void ConfigureLeftSprite(Player * player);
void ConfigureRightSprite(Player * player);
void ConfigureUpSprite(Player * player);
void ConfigureDownSprite(Player * player);
void ConfigureDeadSprite(Player * player);
void Draw_Player(Player * currentPlayer);
void updatePlayerPerimeter(Player * player, Direction direction);
void setHorizontalVelocity(Player * player, int16_t horizontalVelocity);
void setVerticalVelocity(Player * player, int16_t verticalVelocity);
int16_t getHorizontalVelocity(Player * player);
int16_t getVerticalVelocity(Player * player);
int16_t getxMultiplier(Player * player);
int16_t getyMultiplier(Player * player);

void setDirection(Player * player, Direction newDirection);
void setCurrentSprite(Player * player, int16_t newMovementIndex);
int16_t getXPositionCenter(Player * player);
int16_t getYPositionCenter(Player * player);
Direction getDirection(Player * player);
void setXPosition(Player * player, int16_t newX);
void setYPosition(Player * player, int16_t newY);
int16_t getXPosition(Player * player);
int16_t getYPosition(Player * player);
void updateXPosition(Player * player);
void updateYPosition(Player * player);
void placeBomb(Player * player, Bombs * bomb, int16_t bombId, int16_t positionId, int16_t numberofFrames);
void InsertBomb(Player * player, Bombs * bomb);

void setPlayerPositionId(Player * player, int newPositionId);
Player * getPlayer(int16_t positionId);
int16_t getPlayerPositionId(Player * player);
int16_t getPlayerId(Player * player);
State checkPlayerStatus(Player * player);

void setHorizontalVelocityMultiplier(Player * player, int newXMultiplier);
void setVerticalVelocityMultiplier(Player * player, int newYMultiplier);
void determineStateOfLiving(Player * currentPlayer1, Player * currentPlayer2, int16_t positionId);
void DrawPlayerDeath(Player * currentPlayer);

extern Player player1;
extern Player player2;
extern Object objectGrid[][15];
extern const unsigned short P1_DL[];
extern const unsigned short P2_DL[];
#endif




