#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "structures.h"
#include "time.h"

void updateAnimation(Animation* animation);
Rectangle frameAnimation(Animation* anim);

#endif // ANIMATION_H
