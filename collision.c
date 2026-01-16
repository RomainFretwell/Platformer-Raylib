#include "collision.h"

bool rectangleCollision(IntRectangle a, IntRectangle b){
    return (
        a.x < b.x + b.width &&
        a.x + a.width > b.x &&
        a.y < b.y + b.width &&
        a.y + a.width > b.y
    );
}

bool checkCollisionTriangles(Vector2 A1, Vector2 B1, Vector2 C1, Vector2 A2, Vector2 B2, Vector2 C2){
    if (CheckCollisionPointTriangle(A1, A2, B2, C2)) return true;
    if (CheckCollisionPointTriangle(B1, A2, B2, C2)) return true;
    if (CheckCollisionPointTriangle(C1, A2, B2, C2)) return true;
    if (CheckCollisionPointTriangle(A2, A1, B1, C1)) return true;
    if (CheckCollisionPointTriangle(B2, A1, B1, C1)) return true;
    if (CheckCollisionPointTriangle(C2, A1, B1, C1)) return true;
    return false;
}

void rectToPoints(Hitbox *hitbox){
    hitbox->A = (Vector2) {hitbox->x, hitbox->y};
    hitbox->B = (Vector2) {hitbox->A.x + hitbox->width*cosf(hitbox->angle * PI/180), hitbox->A.y + hitbox->width*sinf(hitbox->angle * PI/180)};
    hitbox->D = (Vector2) {hitbox->A.x - hitbox->height*sinf(hitbox->angle * PI/180), hitbox->A.y + hitbox->height*cosf(hitbox->angle * PI/180)};
    hitbox->C = (Vector2) {hitbox->B.x + hitbox->D.x - hitbox->A.x, hitbox->B.y + hitbox->D.y - hitbox->A.y};
}

bool checkCollisionHitboxes(Hitbox hitbox1, Hitbox hitbox2){
    if (checkCollisionTriangles(hitbox1.A, hitbox1.B, hitbox1.C, hitbox2.A , hitbox2.B , hitbox2.C)) return true;
    if (checkCollisionTriangles(hitbox1.A, hitbox1.B, hitbox1.C, hitbox2.A , hitbox2.D , hitbox2.C)) return true;
    if (checkCollisionTriangles(hitbox1.A, hitbox1.D, hitbox1.C, hitbox2.A , hitbox2.B , hitbox2.C)) return true;
    if (checkCollisionTriangles(hitbox1.A, hitbox1.D, hitbox1.C, hitbox2.A , hitbox2.D , hitbox2.C)) return true;
    return false;
}

void updateHitboxEntity(Entity *ent){
    float angle = ent->angle*PI/180 + atanf((ent->texture.height - ent->hitbox.topOffset - ent->hitbox.bottomOffset) / (ent->texture.width - ent->hitbox.leftOffset - ent->hitbox.rightOffset));
    float AB = ent->texture.height - ent->hitbox.bottomOffset - ent->hitbox.topOffset;
    float BC = ent->texture.width - ent->hitbox.leftOffset - ent->hitbox.rightOffset;
    float radius = 0.5f*sqrtf(AB*AB + BC*BC);

    ent->hitbox.x = ent->position.x - radius*cosf(angle);
    ent->hitbox.y = ent->position.y - radius*sinf(angle);
    ent->hitbox.width = ent->texture.width - ent->hitbox.leftOffset - ent->hitbox.rightOffset;
    ent->hitbox.height = ent->texture.height - ent->hitbox.topOffset - ent->hitbox.bottomOffset;
    ent->hitbox.angle = ent->angle;
    rectToPoints(&(ent->hitbox));
}

void updateHitboxItem(Item *item){ // ajouter décalage en parametre
    item->hitbox.x = item->position.x; // à modifier ?
    item->hitbox.y = item->position.y; // à modifier ?
    item->hitbox.width = item->texture.width; // à enlever ?
    item->hitbox.height = item->texture.height; // à enlever ?
    item->hitbox.angle = item->angle;
    rectToPoints(&(item->hitbox));
}

void updatePhysicsBoxEntity(Entity *ent){
    ent->physicsBox.x = ent->position.x - ent->physicsBox.width/2;
    ent->physicsBox.y = ent->position.y - ent->physicsBox.height/2;
}

void indexToIntRectangle(int index, IntRectangle *rect){
    rect->x = blockSize * (index / mapSizeY);
    rect->y = blockSize * (index % mapSizeY);
    rect->width = blockSize;
    rect->height = blockSize;
}

void indexToHitbox(int index, Hitbox *hitbox){
    hitbox->x = blockSize * (index / mapSizeY);
    hitbox->y = blockSize * (index % mapSizeY);
    hitbox->width = blockSize;
    hitbox->height = blockSize;
    hitbox->angle = 0;
    //rectToPoints(&(*hitbox)); // à changer
}

int findBlockMap(Entity ent, int mapSizeX, int mapSizeY){
    int x = ent.position.x / blockSize;
    int y = ent.position.y / blockSize;
    return x%mapSizeX * mapSizeY + y%mapSizeY; // % mapSizeX  ?
}

void handleBlockCollisions(Entity * ent, int map[]){
    Hitbox blockHitbox;
    int n;
    int centerBlock = findBlockMap(*ent, mapSizeX, mapSizeY);
    for (int i = -1; i<2; i++){
        for (int j = -1; j<2; j++){
            n = centerBlock + mapSizeY*i + j;
            if (map[n] != 0){ // 0 = air
                indexToHitbox(n, &blockHitbox);
                //drawHitbox(blockHitbox, BLUE);
                rectToPoints(&blockHitbox);
                if (checkCollisionHitboxes(ent->hitbox, blockHitbox)){ // remplacer par while ensuite
                    while (checkCollisionHitboxes(ent->hitbox, blockHitbox)){
                        ent->position.x -= signe(ent->speed.x);
                        ent->position.y -= signe(ent->speed.y);
                        updateHitboxEntity(ent);
                    }
                    ent->acceleration.x = 0;
                    ent->speed.x = 0;
			    }
		    }
        }
    }
}