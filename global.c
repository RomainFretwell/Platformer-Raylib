#include "global.h"

IntVector2 fullScreenSize;
IntVector2 smallScreenSize = {640, 360};
IntVector2 currentScreenSize;
float screenRatio = 1.0f;

float deltaTime;

bool showBlockHitbox = false;
bool showEntityHitbox = false;

// TEXTURES
Texture2D Texture_background_0;
Texture2D Texture_Block_Atlas;
Texture2D Texture_Player_Idle;
Texture2D Texture_Hologram_Saw;
Texture2D Texture_green_slime_idle;
