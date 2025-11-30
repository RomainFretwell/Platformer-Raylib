#ifndef COLLISION_H
#define COLLISION_H

#include "structures.h"

bool checkCollisionTriangles(Vector2 A1, Vector2 B1, Vector2 C1, Vector2 A2, Vector2 B2, Vector2 C2);
void rectToPoints(Hitbox *hitbox);
bool checkCollisionHitboxes(Hitbox hitbox1, Hitbox hitbox2);
void updateHitboxEntity(Entity *ent);
void updateHitboxItem(Item *item);
void indexToHitbox(int index, Hitbox *hitbox);
void handleBlockCollisions(Entity * ent, int map[]);
int findBlockMap(Entity ent, int mapSizeX, int mapSizeY); // à enlever ?

#endif // COLLISION_H
