// ScoreBoard.h
// Runs on LM4F120/TM4C123
// Brian Tsai, Benson Huang
// Last Modified: 05/04/2017
#include <stdint.h>
#include "Struct.h"


void ScoreBoard_Init(void);
void Clock_Init(void);
void Mascot_Init(void);
void Update_Clock(void);
void Draw_Clock(void);
void Mascot_Init(void);
void Score_Init(void);
void Draw_Score(void);
int8_t ReturnScore(Player * player);
void UpdateScore(Player *player);
void Draw_Mascot(void);
void FlashScreen(int flipColor);

extern const unsigned short P1_Score[];
extern const unsigned short P2_Score[];
