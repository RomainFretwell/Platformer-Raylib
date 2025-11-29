#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "structures.h"

#define nbBlock 5
#define blockSize 15 // les blocks sont de taille 15x15
#define sizeCoef 1 // multiplier blockSize par sizeCoef pour des blocks plus gros

extern Vector2int fullScreenSize;
extern Vector2int smallScreenSize;
extern Vector2int currentScreenSize;
extern float screenRatio;

extern bool showBlockHitbox;
extern bool showEntityHitbox;

extern int mapSizeX;
extern int mapSizeY;

#endif // CONSTANTS_H
