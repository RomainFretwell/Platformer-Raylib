#include "math2.h"

float distance(Vector2 objetA, Vector2 objetB){
    return sqrtf((objetA.x - objetB.x) * (objetA.x - objetB.x) + (objetA.y - objetB.y) * (objetA.y - objetB.y));
}

int signe(float x){
    if (x > 0) return 1;
    else if (x < 0) return -1;
    else return 0;
}