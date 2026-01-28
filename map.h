#ifndef MAP_H
#define MAP_H

#include <stdio.h>
//#include <stdlib.h>
//#include "raylib.h"
#include "structures.h"
#include "constants.h"

void clearMap(Map map);
void LoadMap(Map map, FILE* fp);
void testMap(Map map); // à enlever plus tard
void autoTile(Map map);

#endif // MAP_H
