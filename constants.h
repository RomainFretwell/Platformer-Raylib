#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "raylib.h"
#include "structures.h"

#define blockSize 16
#define MAX_MAP_SIZE_X 500
#define MAX_MAP_SIZE_Y 500

extern IntVector2 fullScreenSize;
extern IntVector2 smallScreenSize;
extern IntVector2 currentScreenSize;
extern float screenRatio;

extern bool showBlockHitbox;
extern bool showEntityHitbox;

#endif // CONSTANTS_H
