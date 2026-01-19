#include "draw.h"

void drawCross(int x, int y, Color color){
    DrawLine(0, y*screenRatio, 2500*screenRatio, y*screenRatio, color); // currentScreenSize.x à la place de 1000
    DrawLine(x*screenRatio, 0, x*screenRatio, 1200*screenRatio, color); // currentScreenSize.y
}

void drawBlock(int x, int y, Block block){
    DrawTextureEx(block.texture, (Vector2){x*blockSize*sizeCoef*screenRatio, y*blockSize*sizeCoef*screenRatio}, 0, sizeCoef*screenRatio, WHITE);
}

void drawBlockHitbox(int x, int y, Color color){
    float hitX = x * blockSize * sizeCoef * screenRatio;
    float hitY = y * blockSize * sizeCoef * screenRatio;
    float hitW = blockSize * sizeCoef * screenRatio;
    float hitH = blockSize * sizeCoef * screenRatio;

    DrawRectangleLinesEx((Rectangle){hitX, hitY, hitW, hitH}, 1.0f, color);
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

void drawEntity(Entity ent){
    // part du principe que la position (x,y) est le centre de la physicsBox (et pas le coin en haut à gauche)
    Rectangle source = (Rectangle) {0, 0, ent.direction * ent.texture.width, ent.texture.height};
    
    Rectangle dest;
    dest.x = ent.position.x * screenRatio;
    dest.y = ent.position.y * screenRatio;
    dest.width = ent.texture.width * screenRatio;
    dest.height = ent.texture.height * screenRatio;

    Vector2 origin;
    if (ent.direction == 1) origin.x = ent.origin.x * screenRatio;
    else if (ent.direction == -1) origin.x = (ent.texture.width - ent.origin.x) * screenRatio;
    origin.y = ent.origin.y * screenRatio;
    
    DrawTexturePro(ent.texture, source, dest, origin, ent.angle, WHITE);
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