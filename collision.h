#ifndef COLLISION_H
#define COLLISION_H

#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "structures.h"
#include "constants.h"
#include "math2.h"
#include "draw.h" // à enlever après ?

bool checkCollisionTriangles(Vector2 A1, Vector2 B1, Vector2 C1, Vector2 A2, Vector2 B2, Vector2 C2);
void rectToPoints(Hitbox *hitbox);
bool checkCollisionHitboxes(Hitbox hitbox1, Hitbox hitbox2);
void updateHitboxEntity(Entity *ent);
void updateHitboxItem(Item *item);
void updatePhysicsBoxEntity(Entity *ent);
void indexToHitbox(int index, Hitbox *hitbox);
void indexToIntRectangle(int index, IntRectangle *rect);
void handleBlockCollisions(Entity * ent, int map[]);
int findBlockMap(Entity ent, int mapSizeX, int mapSizeY); // à enlever ?

#endif // COLLISION_H
