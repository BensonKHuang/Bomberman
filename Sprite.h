// Sprite.h
// Runs on LM4F120/TM4C123
// Brian Tsai, Benson Huang
// Last Modified: 05/04/2017
#ifndef SPRITE_HEADER 
#define SPRITE_HEADER

#include <stdint.h>
#include "Struct.h"

void Sprite_Init(Sprite * sprite, int16_t startingX, int16_t startingY, int16_t numberofFrames, Status type);
void InsertFrame(Sprite * sprite, const unsigned short *frame);
void Update_Sprite(Sprite * sprite);
void Update_Animation(Animation * animation);
const unsigned short* getFrame(Sprite * sprite);
void Draw_Sprite(Sprite * currentSprite);
AnimationStatus getAnimationStatus(Sprite * sprite);

#endif
