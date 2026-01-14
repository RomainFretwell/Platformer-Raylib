#ifndef MOUVEMENT_H
#define MOUVEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "constants.h"
#include "structures.h"
#include "math2.h"
#include "collision.h"

void mouvement(Entity *player, int map[], float dt);

#endif // MOUVEMENT_H
