#include "draw.h"
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "structures.h"
#include "constants.h"


void drawCross(int x, int y, Color color){
    DrawLine(0, y*screenRatio, currentScreenSize.width, y*screenRatio, color);
    DrawLine(x*screenRatio, 0, x*screenRatio, currentScreenSize.height, color);
}

void drawBlock(int x, int y, Block block){
    //int x = i / mapSizeY;
    //int y = i % mapSizeY;
    DrawTextureEx(block.texture, (Vector2){x*blockSize*sizeCoef*screenRatio, y*blockSize*sizeCoef*screenRatio}, 0, sizeCoef*screenRatio, WHITE);
}

void drawBlockHitbox(int x, int y, Color color){
    // changer type Block pour avoir hitbox ? pour pouvoir utiliser collision et drawHitbox...
    //int x = i / mapSizeY;
    //int y = i % mapSizeY;
    DrawLine(x*blockSize*sizeCoef*screenRatio, y*blockSize*sizeCoef*screenRatio+1, (x+1)*blockSize*sizeCoef*screenRatio, y*blockSize*sizeCoef*screenRatio+1, color);
    DrawLine(x*blockSize*sizeCoef*screenRatio+1, (y+1)*blockSize*sizeCoef*screenRatio, (x+1)*blockSize*sizeCoef*screenRatio, (y+1)*blockSize*sizeCoef*screenRatio, color);
    DrawLine(x*blockSize*sizeCoef*screenRatio+1, y*blockSize*sizeCoef*screenRatio+1, x*blockSize*sizeCoef*screenRatio+1, (y+1)*blockSize*sizeCoef*screenRatio, color);
    DrawLine((x+1)*blockSize*sizeCoef*screenRatio, y*blockSize*sizeCoef*screenRatio+1, (x+1)*blockSize*sizeCoef*screenRatio, (y+1)*blockSize*sizeCoef*screenRatio, color);
}

void drawMap(int map[], Block blockID[]){
    int i;
    if (showBlockHitbox){
        for (int x = 0; x < mapSizeX; x++){
            for (int y = 0; y < mapSizeY; y++){
                i = x*mapSizeY + y;
                if (blockID[map[i]].solid){ // if solid && showBlockHitbox (càd pas de l'air ou de l'eau)
                    drawBlock(x, y, blockID[map[i]]);
                    drawBlockHitbox(x, y, BLACK);
                }
            }
        }
    }
    else {
        for (int x = 0; x < mapSizeX; x++){
            for (int y = 0; y < mapSizeY; y++){
                i = x*mapSizeY + y;
                drawBlock(x, y, blockID[map[i]]);
            }
        }
    }
}

void drawHitbox(Hitbox hitbox, Color color){
    // AB
    DrawLine(screenRatio*hitbox.A.x-1, screenRatio*hitbox.A.y, screenRatio*hitbox.B.x, screenRatio*hitbox.B.y, color);
    // BC
    DrawLine(screenRatio*hitbox.B.x, screenRatio*hitbox.B.y, screenRatio*hitbox.C.x, screenRatio*hitbox.C.y, color);
    // CD
    DrawLine(screenRatio*hitbox.C.x, screenRatio*hitbox.C.y, screenRatio*hitbox.D.x, screenRatio*hitbox.D.y, color);
    // AD
    DrawLine(screenRatio*hitbox.A.x, screenRatio*hitbox.A.y, screenRatio*hitbox.D.x, screenRatio*hitbox.D.y, color);
}

void drawEntity(Entity ent){ // part du principe que (x,y) est le centre de la hitbox (et pas le coin en haut à gauche)
    int size = 1;
    DrawTexturePro(
        ent.texture,
        (Rectangle) {0, 0, ent.direction * ent.texture.width, ent.texture.height}, // source
        (Rectangle) {ent.position.x*screenRatio,
                    ent.position.y*screenRatio, // ent.texture.height + ent.hitbox.topOffset - ent.hitbox.bottomOffset)/2
                    size*ent.texture.width*screenRatio,
                    size*ent.texture.height*screenRatio}, // dest
        (Vector2)   {(ent.texture.width + ent.direction*(ent.hitbox.leftOffset - ent.hitbox.rightOffset))/2 * screenRatio, 
                    (ent.texture.height + ent.hitbox.topOffset - ent.hitbox.bottomOffset)/2 * screenRatio}, // origin
        ent.angle,
        WHITE);
}

void drawItem(Item item){
    int size = 1;
    DrawTexturePro( item.texture,
                    (Rectangle) {0, 0, item.texture.width, item.texture.height}, // source
                    (Rectangle) {(item.position.x + item.texture.width/2)*screenRatio,
                                (item.position.y + item.texture.height/2)*screenRatio,
                                size*item.texture.width*screenRatio,
                                size*item.texture.height*screenRatio}, // dest
                    (Vector2) {item.texture.width/2 * screenRatio, item.texture.height/2 * screenRatio}, // origin
                    item.angle, 
                    WHITE);
}