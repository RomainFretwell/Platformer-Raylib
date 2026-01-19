#ifndef TIME_H
#define TIME_H

#include <stdio.h>
//#include <stdlib.h>
#include "raylib.h"

typedef struct{
    float lifetime;
    float timeleft;
} Timer;

void startTimer(Timer* timer);
void endTimer(Timer* timer);
void updateTimer(Timer* timer);
bool timerIsDone(Timer* timer);

#endif // TIME_H
