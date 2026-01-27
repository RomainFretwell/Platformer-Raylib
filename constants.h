#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "structures.h"

#define nbBlock 5
#define blockSize 15 // les blocks sont de taille 15x15
#define sizeCoef 1 // multiplier blockSize par sizeCoef pour des blocks plus gros

extern IntVector2 fullScreenSize;
extern IntVector2 smallScreenSize;
extern IntVector2 currentScreenSize;
extern float screenRatio;

extern bool showBlockHitbox;
extern bool showEntityHitbox;

extern IntVector2 mapSize;

#endif // CONSTANTS_H
