#include "animation.h"

void updateAnimation(Animation* anim){
    updateTimer(&anim->timer);
    if (timerIsDone(&anim->timer)){
        startTimer(&anim->timer);
        anim->current += anim->step;
        if (anim->current > anim->last){
            switch (anim->type){
                case REPEATING:
                    anim->current = anim->first;
                    break;
                case ONESHOT:
                    anim->current = anim->last;
                    break;
                default:
                    printf("Problème type d'animation\n");
                    break;
            }
        }
        else if (anim->current < anim->first){
            switch (anim->type){
                case REPEATING:
                    anim->current = anim->last;
                    break;
                case ONESHOT:
                    anim->current = anim->first;
                    break;
                default:
                    printf("Problème type d'animation\n");
                    break;
            }
        }
    }
}

// renvoie le rectangle source pour draw
Rectangle frameAnimation(Animation* anim){
    int nbFrames = anim->last - anim->first + 1;
    int x = (anim->current % nbFrames) * anim->frameSize.x;
    int y = (anim->current / nbFrames) * anim->frameSize.y;
    return (Rectangle) {(float) x, (float) y, anim->frameSize.x, anim->frameSize.y};
}