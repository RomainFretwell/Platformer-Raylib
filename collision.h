#ifndef COLLISION_H
#define COLLISION_H

#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "structures.h"
#include "constants.h"
#include "math2.h"
#include "draw.h" // à enlever après ?

bool rectangleCollision(IntRectangle a, IntRectangle b);
void indexToIntRectangle(int index, IntRectangle *rect);
int findBlockMap(Entity ent, int mapSizeX, int mapSizeY); // à enlever ?
void updatePhysicsBoxEntity(Entity *ent);

bool checkCollisionTriangles(Vector2 A1, Vector2 B1, Vector2 C1, Vector2 A2, Vector2 B2, Vector2 C2);
bool checkCollisionHitboxes(Hitbox hitbox1, Hitbox hitbox2);
void rectToPoints(Hitbox *hitbox);
void updateHitboxEntity(Entity *ent);

#endif // COLLISION_H
