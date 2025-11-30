#include "camera.h"
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "structures.h"
#include "constants.h"

void limitCameraMap(Camera2D * camera){
    if (camera->target.x < camera->offset.x){
        camera->target.x = camera->offset.x;
    }
    if (camera->target.x > camera->offset.x + mapSizeX*blockSize*screenRatio - currentScreenSize.width){
        camera->target.x = camera->offset.x + mapSizeX*blockSize*screenRatio - currentScreenSize.width;
    }
    if (camera->target.y < camera->offset.y){
        camera->target.y = camera->offset.y;
    }
    if (camera->target.y > camera->offset.y + mapSizeY*blockSize*screenRatio - currentScreenSize.height){
        camera->target.y = camera->offset.y + mapSizeY*blockSize*screenRatio - currentScreenSize.height;
    }
}

void cameraFollow(Camera2D * camera, Entity player){
    float cameraAcceleration = 0.04;
    camera->target.x += cameraAcceleration * (player.position.x*screenRatio - camera->target.x);
    camera->target.y += cameraAcceleration * (player.position.y*screenRatio - camera->target.y);
}

void limitCameraFollow(Camera2D * camera, Entity player){
    int cameraFollowThresh = 120;
    if ((player.position.x - cameraFollowThresh) * screenRatio > camera->target.x){
        camera->target.x = (player.position.x - cameraFollowThresh) * screenRatio;
    }
    if ((player.position.x + cameraFollowThresh) * screenRatio < camera->target.x){
        camera->target.x = (player.position.x + cameraFollowThresh) * screenRatio;
    }
    if ((player.position.y - cameraFollowThresh) * screenRatio > camera->target.y){
        camera->target.y = (player.position.y - cameraFollowThresh) * screenRatio;
    }
    if ((player.position.y + cameraFollowThresh) * screenRatio < camera->target.y){
        camera->target.y = (player.position.y + cameraFollowThresh) * screenRatio;
    }
}

void shakeCamera(Camera2D * camera, float intensity, float duration){
    // TODO
}