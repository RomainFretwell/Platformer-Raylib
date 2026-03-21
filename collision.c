#include "collision.h"

bool rectangleCollision(IntRectangle a, IntRectangle b){
    return (
        a.x < b.x + b.width &&
        a.x + a.width > b.x &&
        a.y < b.y + b.height &&
        a.y + a.height > b.y
    );
}

void indexToIntRectangle(int index, IntRectangle *rect, Map map){
    rect->x = blockSize * (index / map.size.y);
    rect->y = blockSize * (index % map.size.y);
    rect->width = blockSize;
    rect->height = blockSize;
}

int findBlockMap(Entity ent, Map map){
    int x = ent.position.x / blockSize;
    int y = ent.position.y / blockSize;
    return x%map.size.x * map.size.y + y%map.size.y; // % map.size.x  ?
}

void updatePhysicsBoxEntity(Entity *ent){
    ent->physicsBox.x = ent->position.x - ent->physicsBox.width/2;
    ent->physicsBox.y = ent->position.y - ent->physicsBox.height/2;
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

bool checkCollisionHitboxes(Hitbox hitbox1, Hitbox hitbox2){
    if (checkCollisionTriangles(hitbox1.A, hitbox1.B, hitbox1.C, hitbox2.A , hitbox2.B , hitbox2.C)) return true;
    if (checkCollisionTriangles(hitbox1.A, hitbox1.B, hitbox1.C, hitbox2.A , hitbox2.D , hitbox2.C)) return true;
    if (checkCollisionTriangles(hitbox1.A, hitbox1.D, hitbox1.C, hitbox2.A , hitbox2.B , hitbox2.C)) return true;
    if (checkCollisionTriangles(hitbox1.A, hitbox1.D, hitbox1.C, hitbox2.A , hitbox2.D , hitbox2.C)) return true;
    return false;
}

