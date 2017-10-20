// Sound.h
// Runs on LM4F120/TM4C123
// Brian Tsai, Benson Huang
// Last Modified: 05/04/2017

#include <stdint.h>

void Sound_Init(void);
void Sound_Play(int songIndex);
void Sound_SetPeriod(uint32_t period);
void Sound_Shoot(void);
void Sound_Killed(void);
void Sound_Explosion(void);

void Sound_Fastinvader1(void);
void Sound_Fastinvader2(void);
void Sound_Fastinvader3(void);
void Sound_Fastinvader4(void);
void Sound_Highpitch(void);


void MusicArray_Init(void);

extern uint8_t songState; 
extern uint8_t buttonState;


#define	B1 40493 	// 62 Hz
#define C2 38220 	// 65 Hz
#define D2b 36075	// 69 Hz
#define G3	12755	// 196 Hz
#define A3	11367	// 220 Hz
#define B3	10124	// 247 Hz
#define C4 9541		// 262 Hz
#define D4b 9019	// 277 Hz
#define D4 8503		// 294 Hz
#define E4b 8035	// 311 Hz
#define E4 7575		// 330 Hz
#define F4 7163		// 348 Hz
#define G4b 6757	// 370 Hz
#define G4 6377		// 392 Hz
#define A4b 6024  // 415 Hz
#define A4 5681		// 440 Hz
#define B4b	5603	// 466 Hz
#define B4 5060		// 494 Hz
#define C5 4770		// 523 Hz
#define C5s 4780	// 523 Hz
#define D5 4251		// 524 Hz
#define E5b 4019  // 622 Hz
#define E5 3787		// 588 Hz
#define F5 3581	  // 660 Hz
#define G5b 3378  // 740 Hz
#define G5 3188   // 784 Hz
#define A5b 3008  // 831 Hz
#define A5 2840   // 880 Hz
#define B5b 2682  // 932 Hz
#define B5 2530   // 988 Hz
#define C6 2385   // 1046 Hz
#define D6b 2255	// 1109 Hz
#define D6 2128   // 1175 Hz
#define E6b 2009	// 1245 Hz
#define E6 1896   // 1319 Hz
#define F6 1790   // 1397 Hz
#define G6b 1689  // 1480 Hz
#define G6 1594   // 1568 Hz
#define A6b 1505	// 1661 Hz
#define A6 1420   // 1760 Hz
#define B6 1265   // 1976 Hz
#define C7 1194   // 2093 Hz
#define D7b 1127  // 2217 Hz
#define D7 1064		// 2349 Hz
#define E7 948    // 2637 Hz
#define F7 895    // 2794 Hz
#define G7 797    // 3136 Hz
#define A7 710    // 3520 Hz
#define B7 633    // 3951 Hz
#define C8 597    // 4186 Hz
#define D8 532    // 4699 Hz
#define E8 474    // 5274 Hz

#define	quick				2000000	//	26.67 ms
#define sixteenth		5000000	//	12.5 ms
#define eighth  		10000000 // 187.5 ms
#define quarter 		20000000 // 375 ms
#define dot_quarter 30000000 // 562.5 ms
#define half 				40000000 // 7.5 sec
#define dot_half 	 80000000 // 1.125 sec

