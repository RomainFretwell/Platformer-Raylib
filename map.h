#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include "structures.h"
#include "global.h"

void clearMap(Map map);
//void LoadMap(Map map, FILE* fp);
void testMap(Map map); // à enlever plus tard
void autoTile(Map map, int x, int y);
void localAutoTile(Map map, int x, int y);
void completeAutoTile(Map map);

#endif // MAP_H
