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


// Block
typedef struct {
    Texture2D texture;
    bool solid;
    bool breakable;
    float coefRebond;
} Block;


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
