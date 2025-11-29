#ifndef DRAW_H
#define DRAW_H

#include "structures.h"

void drawCross(int x, int y, Color color);
void drawBlock(int x, int y, Block block);
void drawBlockHitbox(int x, int y, Color color);
void drawMap(int map[], Block blockID[]);
void drawHitbox(Hitbox hitbox, Color color);
void drawEntity(Entity ent);
void drawItem(Item item);

#endif // DRAW_H
