// Music.h
// This module contains the Timer1A ISR that plays music score
// Runs on LM4F120 or TM4C123
// Program written by: Brian Tsai & Benson Huang
// Date Created: 3/6/17 
// Last Modified: 3/6/17 
// Lab number: 10!
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Sound.h"

void Music_Init(void);
void Timer3_Init(uint32_t period);
void Timer3A_Start(void);
void Timer3A_Stop(void);

extern uint8_t noteIndex; // Holds the index of noteList 
extern const uint32_t *notePt; // Holds the pointer of noteList
extern uint8_t noteLengthIndex; // Holds the index of noteLengthList 
extern const uint32_t *noteLengthPt; // Holds the pointer of noteLengthList
extern uint32_t arraySize;
extern const uint32_t *currentList;
extern const uint32_t battleList[];
extern const uint32_t battleLengthList[];
extern const uint32_t explosionList[];
extern const uint32_t explosionLengthList[];
extern const uint32_t deathList[];
extern const uint32_t deathLengthList[];
extern const uint32_t powerupList[];
extern const uint32_t powerupLengthList[];
extern const uint32_t menuList[];
extern const uint32_t menuLengthList[];
extern const uint32_t victoryList[];
extern const uint32_t victoryLengthList[];




