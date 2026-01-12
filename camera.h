#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "structures.h"
#include "constants.h"

void limitCameraMap(Camera2D * camera);
void cameraFollow(Camera2D * camera, Entity player);
void limitCameraFollow(Camera2D * camera, Entity player);
void shakeCamera(Camera2D * camera, float intensity, float duration);

#endif // CAMERA_H
