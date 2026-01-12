#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "raylib.h"

typedef struct structVector2int {
    int width;
    int height;
} Vector2int;

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
    // hitbox offset
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
    float VxMax;
    float VyMax;
    float VabsMax;
    float angle;
    int direction;
    Texture2D texture;
    Hitbox hitbox;
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
