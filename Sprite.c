// Sprite.c
// Runs on LM4F120/TM4C123
// Brian Tsai, Benson Huang
// Last Modified: 05/04/2017
#include <stdint.h>
#include <stdlib.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
//#include "SpriteMovements.h"
#include "Sprite.h"





void Sprite_Init(Sprite *sprite, int16_t startingX, int16_t startingY, int16_t numberofFrames, Status type);
void InsertFrame(Sprite * sprite, const unsigned short *frame);
void setHorizontalVelocity(Player * player, int16_t horizontalVelocity);
void setVerticalVelocity(Player * player, int16_t verticalVelocity);
int getHorizontalVelocity(Player * player);
int getVerticalVelocity(Player * player);
void Update_Sprite(Sprite * sprite);
void Update_Animation(Animation * animation);


//variables
Sprite * mainSprite;
Animation * mainAnimation;
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Sprite_Init for Animation~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Sprite_Init(Sprite * sprite, int16_t startingX, int16_t startingY, int16_t numberofFrames, Status type){
	mainSprite = sprite;
	mainAnimation = &(sprite->animation);
	mainAnimation->arraySize = numberofFrames;
	mainAnimation->insertIndex = 0;
	mainAnimation->frameIndex = 0;
	mainSprite->spriteType = type;
}
	
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Insert Frame: Increase Frame Index~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void InsertFrame(Sprite * sprite, const unsigned short *frame){

	mainSprite = sprite;
	mainAnimation = &(sprite->animation);
	if (mainAnimation->insertIndex == mainAnimation->arraySize){
		return;
		}
	else{
		mainAnimation->frames[mainAnimation->insertIndex] = frame;
		mainAnimation->insertIndex ++;
	}
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~get Frame ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
const unsigned short* getFrame(Sprite * sprite){
	mainAnimation = &(sprite->animation);
	return mainAnimation->frames[mainAnimation->frameIndex];
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Update Sprite~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Update_Sprite(Sprite * sprite){
	if (sprite->spriteType == still){
		return;
	}
	else {
		Update_Animation(&(sprite->animation));
	}
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Update_Animation, modular~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Update_Animation(Animation * animation){
	animation->frameIndex  = (animation->frameIndex + 1) % animation->arraySize;

}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Draw_Sprite for Animation~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Draw_Sprite(Sprite * currentSprite) {
	//ST7735_DrawBitmap(getXPosition(currentSprite), getYPosition(currentSprite), getFrame(currentSprite), 8, 8);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Clock_Init~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
AnimationStatus getAnimationStatus(Sprite * sprite){
	if (sprite->animation.frameIndex == sprite->animation.arraySize - 1){
		sprite->animation.frameIndex = 0;
		return Done;
	}
	else{
		return notDone;
	}
}
