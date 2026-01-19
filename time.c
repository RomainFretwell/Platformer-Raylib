#include "time.h"

void startTimer(Timer* timer){
    if (timer != NULL){
        timer->timeleft = timer->lifetime;
        return;
    }
    printf("---> Erreur startTimer\n");
}

void endTimer(Timer* timer){
    if (timer != NULL){
        timer->timeleft = 0.0f;
        return;
    }
    printf("---> Erreur startTimer\n");
}

void updateTimer(Timer* timer){
    if (timer == NULL){
        printf("---> Erreur updateTimer\n");
        return;
    }
    if (timer->timeleft > 0.0f){
        timer->timeleft -= GetFrameTime();
    }
}

bool timerIsDone(Timer* timer){
    if (timer != NULL){
        return timer->timeleft <= 0.0f;
    }
    printf("---> Erreur timerIsDone\n");
    return true;
}