#include "draw.h"

void drawCross(int x, int y, Color color){
    DrawLine(0, y*screenRatio, 2500*screenRatio, y*screenRatio, color); // currentScreenSize.x à la place de 1000
    DrawLine(x*screenRatio, 0, x*screenRatio, 1200*screenRatio, color); // currentScreenSize.y
}

void drawBlock(TileSet tileSet, int x, int y, int material, int variant){
    // vérifie si pas air = -1
    if (material >= 0){
        Rectangle source = (Rectangle){variant*blockSize, material*blockSize, blockSize, blockSize};

        Rectangle dest;
        dest.x = x * blockSize * screenRatio;
        dest.y = y * blockSize * screenRatio;
        dest.width = blockSize * screenRatio;
        dest.height = blockSize * screenRatio;

        DrawTexturePro(tileSet.texture, source, dest, (Vector2){0,0}, 0, WHITE);
    }
}

void drawBlockHitbox(int x, int y, Color color){
    float hitX = x * blockSize * screenRatio;
    float hitY = y * blockSize * screenRatio;
    float hitW = blockSize * screenRatio;
    float hitH = blockSize * screenRatio;

    DrawRectangleLinesEx((Rectangle){hitX, hitY, hitW, hitH}, 1.0f, color);
}

void drawMap(Map map){
    int index;
    int material;
    int variant;

    if (showBlockHitbox){
        for (int x = 0; x < map.size.x; x++){
            for (int y = 0; y < map.size.y; y++){
                index = x*map.size.y + y;
                material = map.tiled[index] / map.tileSet.size.x;
                variant = map.tiled[index] % map.tileSet.size.x;
                if (map.tiled[index] >= 0){ // if solid (càd pas de l'air ou de l'eau)
                    drawBlock(map.tileSet, x, y, material, variant);
                    drawBlockHitbox(x, y, BLACK);
                }
            }
        }
    }
    else {
        for (int x = 0; x < map.size.x; x++){
            for (int y = 0; y < map.size.y; y++){
                index = x*map.size.y + y;
                material = map.tiled[index] / map.tileSet.size.x;
                variant = map.tiled[index] % map.tileSet.size.x;
                if (map.tiled[index] >= 0){ // if solid (càd pas de l'air ou de l'eau)
                    drawBlock(map.tileSet, x, y, material, variant);
                }
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
    Rectangle source = frameAnimation(&ent.animation);
    source.width *= ent.direction;

    Rectangle dest;
    dest.x = ent.position.x * screenRatio;
    dest.y = ent.position.y * screenRatio;
    dest.width = ent.animation.frameSize.x * screenRatio;
    dest.height = ent.animation.frameSize.y * screenRatio;

    Vector2 origin;
    if (ent.direction == RIGHT) origin.x = ent.animation.origin.x * screenRatio;
    else if (ent.direction == LEFT) origin.x = (ent.animation.frameSize.x - ent.animation.origin.x) * screenRatio;
    else printf("erreur drawEntity\n");
    origin.y = ent.animation.origin.y * screenRatio;
    
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