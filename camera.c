#include "camera.h"

void limitCameraMap(Camera2D * camera){
    if (camera->target.x < camera->offset.x){
        camera->target.x = camera->offset.x;
    }
    if (camera->target.x > camera->offset.x + mapSizeX*blockSize*screenRatio - currentScreenSize.x){
        camera->target.x = camera->offset.x + mapSizeX*blockSize*screenRatio - currentScreenSize.x;
    }
    if (camera->target.y < camera->offset.y){
        camera->target.y = camera->offset.y;
    }
    if (camera->target.y > camera->offset.y + mapSizeY*blockSize*screenRatio - currentScreenSize.y){
        camera->target.y = camera->offset.y + mapSizeY*blockSize*screenRatio - currentScreenSize.y;
    }
}

void cameraFollow(Camera2D * camera, Entity player){
    float cameraAcceleration = 0.04f;
    camera->target.x += cameraAcceleration * (player.position.x*screenRatio - camera->target.x);
    camera->target.y += cameraAcceleration * (player.position.y*screenRatio - camera->target.y);
}

void cameraFollow2(Camera2D * camera, Entity player, float dt){
    float cameraAcceleration = 0.04f;
    float offset = 100;
    float cameraSpeed = 100;
    
    if (player.speed.x == 0){
        camera->target.x = approach(camera->target.x, player.position.x*screenRatio, cameraSpeed * dt);
    }
    else {
        camera->target.x = approach(camera->target.x, (player.position.x + player.direction*offset)*screenRatio, cameraSpeed * dt);
    }
    
    camera->target.y += cameraAcceleration * (player.position.y*screenRatio - camera->target.y);
}

void limitCameraFollow(Camera2D * camera, Entity player, int cameraFollowThresh){
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