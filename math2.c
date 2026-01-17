#include "math2.h"

float distance(IntVector2 A, IntVector2 B){
    return sqrtf((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y));
}

float distanceFloat(Vector2 A, Vector2 B){
    return sqrtf((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y));
}

float signe(float x){
    if (x > 0) return 1.0f;
    else if (x < 0) return -1.0f;
    else return 0.0f;
}

int signInt(int x){
    return (x > 0)?1:-1;
}

float min(float a, float b){
    return (a<b)?a:b;
}

float max(float a, float b){
    return (a>b)?a:b;
}

int arrondir(float x){
    if (x >= 0.0f){
        return (int) (x + 0.5f);
    }
    return (int) (x - 0.5f);
}

float approach(float current, float target, float increase){
    if (current < target){
        return min(current + increase, target);
    }
    return max(current - increase, target);
}