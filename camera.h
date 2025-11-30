#ifndef CAMERA_H
#define CAMERA_H

#include "structures.h"

void limitCameraMap(Camera2D * camera);
void cameraFollow(Camera2D * camera, Entity player);
void limitCameraFollow(Camera2D * camera, Entity player);

#endif // CAMERA_H
