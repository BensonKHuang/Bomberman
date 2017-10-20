// MusicFiles.c
// Runs on LM4F120/TM4C123
// Brian Tsai, Benson Huang
// Last Modified: 05/04/2017
#include <stdint.h>
#include "Sound.h"
#define victorysize  7
#define battlethemesize 212
#define explosionsize  4 
#define sizedeath  9 
#define menuthemesize 125
#define powerupsize  8



const uint32_t battleList[battlethemesize] = {D6, A5, G5b, G5, A5, G5, A5, B5b, C6, D6, E6, F6, G6, A6, F6, C6, A5, C6, D6, C6, D6, E6, E6, 
D6, A5, G5b, G5, A5, G5, A5, B5b, C6, D6, E6, F6, G6, A6, F6, C6, G6, F6, E6, E6, A6,
D6, A5, G5b, G5, A5, G5, A5, B5b, C6, D6, E6, F6, G6, A6, F6, C6, A5, C6, D6, C6, D6, E6, E6, 
D6, A5, G5b, G5, A5, G5, A5, B5b, C6, D6, E6, F6, G6, A6, F6, C6, G6, F6, E6, E6, A6,

D6, G6b, D6, G6b, D6, A5, G5b, F6, D6, B5, D6, B5, G5, D5, G5, A6, G6b, D6, G6b, D6, A5, G5b, F6, D6, B5, D6, B5, G5, D5, 
D6, G6b, D6, G6b, D6, A5, G5b, F6, D6, B5, D6, B5, G5, D5, G5, A6, G6b, D6, G6b, D6, A5, G5b, F6, D6, B5, D6, B5, G5, D5,

G3, G4, B4b, D5, G5, A5, B5b, D6, C6, B5b, A5, G5, F5, E5, D5, C5, 
A3, A4, C5, E5, A5, B5, C6, E6, D6, C6, B5, A5, G5, F5, E5, D5, 
G3, G4, B4b, D5, G5, A5, B5b, D6, C6, B5b, A5, G5, F5, E5, F5, G5, F5, C5, D5,
G3, G4, B4b, D5, G5, A5, B5b, D6, C6, C6, A4, A4, D5, G5b, D5};
	
const uint32_t battleLengthList [battlethemesize] = {eighth, eighth, sixteenth, sixteenth, sixteenth, eighth, sixteenth, sixteenth ,sixteenth ,sixteenth ,sixteenth ,sixteenth ,sixteenth, eighth, eighth, sixteenth, sixteenth, sixteenth, eighth, sixteenth, sixteenth, sixteenth, quarter,
eighth, eighth, sixteenth, sixteenth, sixteenth, eighth, sixteenth, sixteenth ,sixteenth ,sixteenth ,sixteenth ,sixteenth ,sixteenth, eighth, eighth, sixteenth, sixteenth, sixteenth, sixteenth, quarter, quarter, 
eighth, eighth, sixteenth, sixteenth, sixteenth, eighth, sixteenth, sixteenth ,sixteenth ,sixteenth ,sixteenth ,sixteenth ,sixteenth, eighth, eighth, sixteenth, sixteenth, sixteenth, eighth, sixteenth, sixteenth, sixteenth, quarter, 
eighth, eighth, sixteenth, sixteenth, sixteenth, eighth, sixteenth, sixteenth ,sixteenth ,sixteenth ,sixteenth ,sixteenth ,sixteenth, eighth, eighth, sixteenth, sixteenth, sixteenth, sixteenth, quarter, quarter, 

sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, eighth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, eighth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, eighth, 
sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, eighth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, eighth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, eighth,  

sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth,
sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, sixteenth, 
sixteenth,  sixteenth,  sixteenth,  sixteenth,  sixteenth,  sixteenth,  sixteenth,  sixteenth,  sixteenth,  sixteenth,  sixteenth,  sixteenth,  sixteenth,  sixteenth,  sixteenth,  sixteenth, sixteenth, sixteenth, quarter,
sixteenth,  sixteenth,  sixteenth,  sixteenth,  sixteenth,  sixteenth,  sixteenth,  sixteenth, quarter, quarter, quarter, sixteenth, sixteenth, sixteenth, sixteenth }; 
	
const uint32_t explosionList[explosionsize] = {F4, E4, E4b, D4};
const uint32_t explosionLengthList[explosionsize] = {sixteenth, sixteenth, eighth, eighth};
const uint32_t deathList[sizedeath] = {C4, D4b, D4, E4b, E4, F4, G4b, G4, A4b};
const uint32_t deathLengthList[sizedeath] = {sixteenth,sixteenth,sixteenth, sixteenth, sixteenth,sixteenth,sixteenth,sixteenth, eighth};
const uint32_t menuList[menuthemesize] = {A5, D6, A6, A6,
A5, D6, A6, A6b, G6b, E6, B5, A5b, G5b, E5, B4,

D6, G6, D7, D7,
D6, G6, D7, D7b, B6, A6, G6, G6b, E6, D6, D6b,  

A5, D6, A6, A6,
A5, D6, A6, A6b, G6b, E6, B5, A5b, G5b, E5, B4,

D6, G6, D7, D7,
D6, G6, D7, D7b, B6, A6, G6, G6b, E6, D6, D6b,  


D6b, D6, G6b, A6, D7, D6b, D6, G6b, A6, D7,

E6b, E6, A6b, B6, E7, E6b, E6, A6b, B6, E7,

G6b, G6, B6, D7, G7, G6b, G6, B6, D7, G7, A7, A7,
		
		
D6b, D6, G6b, A6, D7, D6b, D6, G6b, A6, D7,

E6b, E6, A6b, B6, E7, E6b, E6, A6b, B6, E7,

G6b, G6, B6, D7, G7, G6b, G6, B6, D7, G7, A7, A7, A7};
const uint32_t menuLengthList[menuthemesize] = {eighth, eighth, eighth, eighth,
eighth,eighth,eighth,eighth,eighth,eighth,eighth,eighth,eighth,eighth, quarter,

eighth, eighth, eighth, eighth,
eighth,eighth,eighth,eighth,eighth,eighth,eighth,eighth,eighth,eighth, quarter,

eighth, eighth, eighth, eighth,
eighth,eighth,eighth,eighth,eighth,eighth,eighth,eighth,eighth,eighth, quarter,

eighth, eighth, eighth, eighth, 
eighth,eighth,eighth,eighth,eighth,eighth,eighth,eighth,eighth,eighth, eighth,

sixteenth,sixteenth,sixteenth,sixteenth, quarter, sixteenth,sixteenth,sixteenth,sixteenth, quarter, 
sixteenth,sixteenth,sixteenth,sixteenth, quarter, sixteenth,sixteenth,sixteenth,sixteenth, quarter,
sixteenth,sixteenth,sixteenth,sixteenth, quarter, sixteenth,sixteenth,sixteenth,sixteenth, quarter, half, half,
 
sixteenth,sixteenth,sixteenth,sixteenth, quarter, sixteenth,sixteenth,sixteenth,sixteenth, quarter, 
sixteenth,sixteenth,sixteenth,sixteenth, quarter, sixteenth,sixteenth,sixteenth,sixteenth, quarter,
sixteenth,sixteenth,sixteenth,sixteenth, quarter, sixteenth,sixteenth,sixteenth,sixteenth, quarter, half, half, eighth};
const uint32_t powerupList[powerupsize] = {E7, F7, G7, A7, B7, C8, D8, E8};
const uint32_t powerupLengthList[powerupsize] = {quick, quick, quick, quick, quick, quick, quick,sixteenth};
const uint32_t victoryList[victorysize] = {D7b, C7, D7b, C7, D7b, C7, D7b};
const uint32_t victoryLengthList[victorysize] = {sixteenth,sixteenth,sixteenth, sixteenth, sixteenth,sixteenth,quarter};
