// Struct.h
// Runs on LM4F120/TM4C123
// Brian Tsai, Benson Huang
// Last Modified: 05/04/2017

#ifndef STRUCT_HEADER
#define STRUCT_HEADER
#include <stdint.h>
//Animation struct
typedef enum {notDone, Done} AnimationStatus;
struct Animation {
	
	const unsigned short* frames[6];
	int16_t arraySize;
	int16_t insertIndex;
	int16_t frameIndex;
};
typedef struct Animation Animation;

//Sprite struct
typedef enum {still, moving} Status;

struct Sprite {
	int16_t movementIndex;
	int16_t size;
	Status spriteType;
	Animation animation;
};
typedef struct Sprite Sprite;



typedef enum {blank, filled} Color;

struct Snapshot {
	int16_t pixelXPosition;
	int16_t pixelYPosition;
	unsigned short color;
	Color isEmpty;
};
typedef struct Snapshot Snapshot;

typedef enum {notExploded, exploded, exploding } ExplosionStatus;
typedef enum {None, Block_Object, Player_Object, Bomb_Object, PowerUp_Object} CharacterType;  
typedef enum {inRange, outOfRange} Range;
typedef enum {notSet, set} BombSet;
struct Bombs {
	CharacterType up;
	CharacterType down;
	CharacterType left;
	CharacterType right;
	CharacterType up2;
	CharacterType down2;
	CharacterType left2;
	CharacterType right2;
	int16_t xPosition;
	int16_t yPosition;
	int16_t xPositionCenter;
	int16_t yPositionCenter;
	int16_t bombId;
	int16_t bombPlayer;
	int16_t positionId;
	int16_t detonationTime;
	Snapshot snapshotFrame [8][8];
	Animation animation;
	Sprite currentSprite;
	Sprite explosionCenter;
	//Sprite explosionUp;
	//Sprite explosionDown;
	//Sprite explosionLeft;
	//Sprite explosionRight;
	ExplosionStatus hasExploded;
	BombSet firstTime;
	int16_t rangeOfExplosion;
};
typedef struct Bombs Bombs;



// datatype indicates availability of position
typedef enum {unoccupied, occupied} Availability;
typedef enum {canBreak, cannotBreak} Destructible;
typedef enum {GrassBlock, HardBlock, SoftBlock} Type;
typedef enum {NoPowerUp, PowerBomb, PowerSkate, PowerFire} PowerUpType;
typedef enum {uncovered, covered} Hidden;
// struct objects
struct Block {
	int16_t xPosition;
	int16_t yPosition;
	int16_t xPositionCenter;
	int16_t yPositionCenter;
	int16_t size;
	Availability canOccupy;
	unsigned short const *blockImage;
	//Destructible canDestroy;
	int16_t idNumber;
	Type blockType;
	PowerUpType powerUp;
	const unsigned short * powerUpImage;
	Hidden hiddenStatus;
};
typedef struct Block Block;


typedef enum {notUsed, inUse, used} Effect;
struct PowerUp {
	int16_t xPosition;
	int16_t yPosition;
	PowerUpType powerUp;
	int16_t powerUpId;
	int16_t powerUpPositionId;
	int16_t effectTime;
	Effect powerUpStatus;
	int16_t powerUpPlayer;
};
typedef struct PowerUp PowerUp;	



//Player struct
typedef enum {standing, up, down, left, right, dead} Direction;
typedef enum {alive, notAlive, kamikaze, finished} State;
struct Player {
	int16_t xPosition;
	int16_t yPosition;
	int16_t xPositionCenter;
	int16_t yPositionCenter;
	int16_t xVelocity;
	int16_t yVelocity;
	int16_t xMultiplier;
	int16_t yMultiplier;
	int16_t playerId;
	int16_t playerPositionId;
	int16_t soundTimer;
	State stateOfLiving;
	Sprite currentSprite;
	Sprite movement[6];
	int16_t movementIndex;
	PowerUpType playerPowerUp;
	PowerUp* currentPowerUp;
	Direction direction;
};

typedef struct Player Player;

//Window struct
struct Window {
	
	Block * topBlock;
	Block * bottomBlock;
	Block * leftBlock;
	Block * rightBlock;
	
	int16_t topBlockId;
	int16_t bottomBlockId;
	int16_t leftBlockId;
	int16_t rightBlockId;
	Player * tracedPlayer;
	int16_t playerGridId;
};
typedef struct Window Window;

typedef enum {clear, contact} Hit;


typedef enum {empty, notEmpty, full} InventoryStatus;

struct Inventory {
	Bombs inventoryBag [7];
	InventoryStatus bagStatus;
	int16_t inventorySize;
	int16_t detonationIndex;
	int16_t getBombIndex;
	int16_t putBombIndex;
	
	//Bombs powerInventoryBag [7];
	//InventoryStatus powerBagStatus;
	//int16_t powerInventorySize;
	//int16_t powerDetonationIndex;
	//int16_t powerGetBombIndex;
	//int16_t powerPutBombIndex;
};
typedef struct Inventory Inventory;

struct Object{
	int16_t xPosition;
	int16_t yPosition;
	int16_t positionId;
	int16_t playerId;
	int16_t bombId;
	int16_t bombPlayer;
	ExplosionStatus hasExploded;
	
}; 
typedef struct Object Object;




struct Supply {
	PowerUp supplyBag [11];
	InventoryStatus supplyStatus;
	int16_t supplySize;
	int16_t newPowerUpIndex;
	int16_t getPowerUpIndex;
	int16_t putPowerUpIndex;
};
typedef struct Supply Supply;


#endif
