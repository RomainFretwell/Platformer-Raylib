#ifndef MATH2_H
#define MATH2_H

#include "raylib.h"
#include <math.h>
#include "structures.h"

float distance(IntVector2 A, IntVector2 B);
float distanceFloat(Vector2 A, Vector2 B);
float signe(float x);
int signInt(int x);
float absf(float x);
float min(float a, float b);
float max(float a, float b);
int arrondir(float x);
int constrain(int value, int min, int max);
float constrainf(float value, float min, float max);
float approach(float current, float target, float increase);
float tweenLinear(float t, float duree, float initialValue, float finalValue);
float tweenPow(float t, float duree, float initialValue, float finalValue, float ordre);
float tweenRoot(float t, float duree, float initialValue, float finalValue, float ordre);
float tweenSmooth(float t, float duree, float initialValue, float finalValue, float ordre);

#endif // MATH2_H
