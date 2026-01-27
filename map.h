#ifndef MAP_H
#define MAP_H

#include <stdio.h>
//#include <stdlib.h>
//#include "raylib.h"
#include "constants.h"

void clearMap(int map[]);
void initializeMap(int map[], FILE* fp);
void testMap(int map[]); // à enlever plus tard
void autoTile(bool** tab, int** tiled);

#endif // MAP_H
