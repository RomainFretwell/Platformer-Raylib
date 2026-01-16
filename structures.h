#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "raylib.h"

typedef struct structIntVector2 {
    int x;
    int y;
} IntVector2;

typedef struct structIntRectangle {
    int x;
    int y;
    int width;
    int height;
} IntRectangle;

// renomer type Hitbox en OrientedRectangle, RotatedRectangle ou autre chose
typedef struct structHitbox {
    float x;
    float y;
    float width;
    float height;
    float angle;

    // hitbox offset à modifier ?
    float bottomOffset;
    float topOffset;
    float leftOffset;
    float rightOffset;

    // autre representation d'un rectangle ABCD avec 4 points
    Vector2 A;
    Vector2 B;
    Vector2 C;
    Vector2 D;
} Hitbox; // rectangle with rotation

typedef struct structEntity {
    IntVector2 position;
    Vector2 speed;
    Vector2 acceleration;

    // sûrement à modifier... ou enlever et mettre direct dans fonctions car constant ?
    float VxMax;
    float VyMax;
    float VabsMax;
    //const float runSpeed;
    //const float runAcceleration;
    //const float runReduce;
    //const float flyReduce;
    //const float fallSpeed;
    //const float jumpSpeed;
    
    float angle;
    int direction;
    Texture2D texture;
    int animationState; // idle + walking + runing/dashing + jumping + dying + ... = choix en vertical sur la texture atlas
    
    IntRectangle physicsBox; // pour collisions avec map
    Hitbox hitbox; // pour collisions avec entity
    
    Vector2 remain;
    bool grounded;
    
    // pour arc et flèche... à mettre autre part
    bool grabbed; 
} Entity;

typedef struct structBlock {
    Texture2D texture;
    bool solid;
    bool breakable;
    float coefRebond;
} Block;

typedef struct structItem {
    IntVector2 position;
    Vector2 speed;
    Vector2 acceleration;
    float angle;
    Texture2D texture;
    Hitbox hitbox;
} Item;

#endif // STRUCTURES_H
