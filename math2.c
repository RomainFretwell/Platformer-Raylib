#include "math2.h"

float distance(Vector2 objetA, Vector2 objetB){
    return sqrtf((objetA.x - objetB.x) * (objetA.x - objetB.x) + (objetA.y - objetB.y) * (objetA.y - objetB.y));
}

float signe(float x){
    if (x > 0) return 1.0f;
    else if (x < 0) return -1.0f;
    else return 0.0f;
}

float min(float a, float b){
    return (a<b)?a:b;
}

float max(float a, float b){
    return (a>b)?a:b;
}

float approach(float current, float target, float increase){
    if (current < target){
        return min(current + increase, target);
    }
    return max(current - increase, target);
}