#ifndef MATH2_H
#define MATH2_H

#include "raylib.h"
#include <math.h>
#include "structures.h"

float distance(Vector2 objetA, Vector2 objetB);
float signe(float x);
float min(float a, float b);
float max(float a, float b);
float approach(float current, float target, float increase);


#endif // MATH2_H
