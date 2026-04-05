#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "structures.h"
#include "global.h"
#include "math2.h"

void limitCameraMap(Camera2D * camera, Map map);
void cameraFollow(Camera2D * camera, Entity player);
void cameraFollow2(Camera2D * camera, Entity player);
void limitCameraFollow(Camera2D * camera, Entity player, int cameraFollowThresh);
Vector2 cameraLookAhead(int amount, Entity player);
//void shakeCamera(Camera2D * camera, float intensity, float duration);

#endif // CAMERA_H
