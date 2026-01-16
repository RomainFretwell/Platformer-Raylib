#ifndef MATH2_H
#define MATH2_H

#include "raylib.h"
#include <math.h>
#include "structures.h"

float distance(IntVector2 A, IntVector2 B);
float signe(float x);
int signInt(int x);
float min(float a, float b);
float max(float a, float b);
int arrondir(float x);
float approach(float current, float target, float increase);


#endif // MATH2_H
