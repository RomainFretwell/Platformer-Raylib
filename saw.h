#ifndef SAW_H
#define SAW_H

#include "structures.h"
#include "math2.h"
#include "time.h"

void initSawTimers(Saw* saw);
void moveSaw(Saw* saw, float ordre);
void initSawCircleFormation(int nbSaw, Saw sawTab[], IntVector2 pos, int rayon, int skip, int speed, Texture2D texture);

#endif // SAW_H
