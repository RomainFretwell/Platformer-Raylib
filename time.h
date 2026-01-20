#ifndef TIME_H
#define TIME_H

#include <stdio.h>
#include "raylib.h"
#include "structures.h"

void startTimer(Timer* timer);
void endTimer(Timer* timer);
void updateTimer(Timer* timer);
bool timerIsDone(Timer* timer);

#endif // TIME_H
