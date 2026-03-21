#include "saw.h"

void initSawTimers(Saw* saw){
    for (int i = 0; i<saw->nbMoves; i++){
        float d = distance(saw->posTab[i%saw->nbMoves], saw->posTab[(i+1)%saw->nbMoves]);
        saw->timeTab[i] = (Timer){d/saw->speed, 0.0f};
    }
}

void moveSaw(Saw* saw, float ordre){
    updateTimer(&saw->timeTab[saw->moveState]);
    if (timerIsDone(&saw->timeTab[saw->moveState])){
        saw->moveState = (saw->moveState + 1)%saw->nbMoves;
        startTimer(&saw->timeTab[saw->moveState]);
    }
    saw->position.x = tweenSmooth(saw->timeTab[saw->moveState].lifetime - saw->timeTab[saw->moveState].timeleft, saw->timeTab[saw->moveState].lifetime, saw->posTab[(saw->moveState)%saw->nbMoves].x, saw->posTab[(saw->moveState + 1)%saw->nbMoves].x, ordre);
    saw->position.y = tweenSmooth(saw->timeTab[saw->moveState].lifetime - saw->timeTab[saw->moveState].timeleft, saw->timeTab[saw->moveState].lifetime, saw->posTab[(saw->moveState)%saw->nbMoves].y, saw->posTab[(saw->moveState + 1)%saw->nbMoves].y, ordre);
}

void initSawCircleFormation(int nbSaw, Saw sawTab[], IntVector2 pos, int rayon, int skip, int speed, Texture2D texture){
    for (int i = 0; i < nbSaw; i++){
        for (int j = 0; j < nbSaw; j++){
            int n = skip*j + i;
            sawTab[i].posTab[j] = (IntVector2){pos.x + rayon*sinf(n*2*PI/nbSaw), pos.y + rayon*cosf(n*2*PI/nbSaw)};
        }
        // animation
        sawTab[i].animState = IDLE,
        sawTab[i].animation.type = REPEATING;
        sawTab[i].animation.first = 0;
        sawTab[i].animation.last = 0; // nbFrames - 1
        sawTab[i].animation.current = 0;
        sawTab[i].animation.step = 1;
        sawTab[i].animation.frameSize = (IntVector2){32,32};
        sawTab[i].animation.origin = (IntVector2){16, 16};
        sawTab[i].animation.timer = (Timer){0.15f, 0.0f};
        sawTab[i].texture = texture;
        sawTab[i].moveState = 0;
        sawTab[i].nbMoves = nbSaw;
        sawTab[i].speed = speed;
        initSawTimers(&(sawTab[i]));
    }
}