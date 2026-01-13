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
    Vector2 position;
    Vector2 speed;
    Vector2 acceleration;

    // sûrement à modifier...
    float VxMax;
    float VyMax;
    float VabsMax;

    float angle;
    int direction;
    Texture2D texture;
    IntRectangle rect; // pour collisions avec map
    Hitbox hitbox; // pour collisions avec entity
    
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
    Vector2 position;
    Vector2 speed;
    Vector2 acceleration;
    float angle;
    Texture2D texture;
    Hitbox hitbox;
} Item;

#endif // STRUCTURES_H
