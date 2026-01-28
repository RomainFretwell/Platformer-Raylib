#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "raylib.h"


// IntVector2
typedef struct {
    int x;
    int y;
} IntVector2;


// IntRectangle
typedef struct {
    int x;
    int y;
    int width;
    int height;
} IntRectangle;


// Hitbox
// renomer type Hitbox en OrientedRectangle, RotatedRectangle ou autre chose
typedef struct {
    float x;
    float y;
    float width;
    float height;
    float angle;

    // autre representation d'un rectangle ABCD avec 4 points
    Vector2 A;
    Vector2 B;
    Vector2 C;
    Vector2 D;
} Hitbox; // rectangle with rotation


// Direction
typedef enum {
    LEFT = -1,
    RIGHT = 1
} Direction;


// AnimationType
typedef enum {
    REPEATING = 0,
    ONESHOT = 1
} AnimationType;


// AnimationState
typedef enum {
    IDLE = 0,
    RUNNING = 1,
    JUMPING = 2,
    WALLSLIDING = 3,
    GLIDING = 4,
    ATTACKING = 5,
    DYING = 6,
} AnimationState;


// Timer
typedef struct{
    float lifetime;
    float timeleft;
} Timer;


// Animation
typedef struct{
    AnimationType type;
    int first;
    int last;
    int current;
    int step;    
    IntVector2 frameSize;
    IntVector2 origin;
    Timer timer; // timer for frames per seconds
} Animation;


// Entity
typedef struct {
    IntVector2 position;
    Vector2 speed;

    //const float runSpeed;
    //const float runAcceleration;
    //const float runReduce;
    //const float flyReduce;
    //const float fallSpeed;
    //const float jumpSpeed;
    
    float angle;
    Direction direction;

    Hitbox hitbox; // pour collisions avec entity
    IntRectangle physicsBox; // pour collisions avec map
    Texture2D texture;
    AnimationState animState;
    Animation animation;
    
    Vector2 remain;
    bool grounded;
} Entity;

// BlockVariant
typedef enum {
    
	FULL_WALL = 0,

	// R=Right  L=Left  U=Up  D=Down  C=Corner
	R_WALL = 5,
	L_WALL = 4,
	U_WALL = 1,
	D_WALL = 2,
	
	RU_WALL = 9,
	LU_WALL = 8,
	RD_WALL = 11,
	LD_WALL = 10,
	
	RLU_WALL = 16,
	RL_WALL = 17,
	RLD_WALL = 18,
	
	UDR_WALL = 7,
	UD_WALL = 3,
	UDL_WALL = 6,
	
	RLUD_WALL = 19,
	
	CRU_WALL = 13,
	CLU_WALL = 12,
	CRD_WALL = 15,
	CLD_WALL = 14,
	
} Variant;

typedef struct{
    Texture2D texture;
    IntVector2 size;
} TileSet;

typedef struct{
	int worldType; // forêt, glace, volcan, chateau en ruines, ...
    TileSet tileSet;

	IntVector2 size;
    int* data; // data[MAX_MAP_SIZE_X * MAX_MAP_SIZE_Y];
    int* tiled; // tiled[MAX_MAP_SIZE_X * MAX_MAP_SIZE_Y];
} Map;

// Block
//typedef struct {
//    Texture2D texture;
//    bool solid;
//    bool breakable;
//    float coefRebond;
//} Block;


// Item
typedef struct {
    IntVector2 position;
    Vector2 speed;
    Vector2 acceleration;
    float angle;
    Texture2D texture;
    Hitbox hitbox;
} Item;


#endif // STRUCTURES_H
