#ifndef DRAW_H
#define DRAW_H

#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "structures.h"
#include "constants.h"
#include "animation.h"

void drawCross(int x, int y, Color color);
void drawBlock(TileSet tileSet, int x, int y, int material, int variant);
void drawBlockHitbox(int x, int y, Color color);
void drawMap(Map map);
void drawHitbox(Hitbox hitbox, Color color);
void drawEntity(Entity ent);
void drawItem(Item item);

#endif // DRAW_H
