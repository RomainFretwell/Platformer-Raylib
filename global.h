#ifndef GLOBAL_H
#define GLOBAL_H

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

// TEXTURES
extern Texture2D Texture_background_0;
extern Texture2D Texture_Block_Atlas;
extern Texture2D Texture_Player_Idle;
extern Texture2D Texture_Hologram_Saw;
extern Texture2D Texture_bow;
extern Texture2D Texture_arrow;
extern Texture2D Texture_green_slime_idle;


#endif // GLOBAL_H
