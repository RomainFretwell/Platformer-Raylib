#include "mouvement.h"

//constantes ici ok ??
static Vector2 remain = {0.0f,0.0f}; // static -> n'initialise remain qu'une fois puis le retient lors du prochain appel de mouvement
static bool grounded = false;
const float runSpeed = 2.0f;
const float runAcceleration = 10.0f;
const float runReduce = 22.0f; 
const float flyReduce = 12.0f;    
const float gravity = 13.0f;
const float fallSpeed = 3.6f;
const float jumpSpeed = -3.0f;

/*// à faire
bool isGrounded(Entity ent, int map[]){
    //return ||  ||  || ;
}

bool isSliding(Entity ent, int map[]){
    //return ||  ||  || ;
}
*/

// 2e argument = truc collide ??
static void moveX(Entity *ent, int map[]){
    remain.x += ent->speed.x;
    int moveX = arrondir(remain.x);
    if(moveX != 0){
        remain.x -= moveX;
        int moveSign = signInt(moveX); // droite ou gauche
        bool collisionHappened = false;

        // bouge ent jusqu'à une collision
        while(!collisionHappened && moveX != 0){
            ent->physicsBox.x += moveSign;

            IntRectangle block;
            int centerBlock = findBlockMap(*ent, mapSizeX, mapSizeY);
            for (int i = -1; i<2; i++){
                for (int j = -1; j<2; j++){
                    int n = centerBlock + mapSizeY*i + j;
                    if (map[n] != 0){ // 0 = air
                        indexToIntRectangle(n, &block);
                        if (rectangleCollision(ent->physicsBox, block)){
                            ent->speed.x = 0;
                            collisionHappened = true;
                            break;
	        		    }
	        	    }
                    if (collisionHappened) break;
                }
                if (collisionHappened) break;
            }
            if (collisionHappened) break;
            
            // bouge si pas de collision
            ent->position.x += moveSign;
            moveX -= moveSign;
        }
    }
}


static void moveY(Entity *ent, int map[]){
    remain.y += ent->speed.y;
    int moveY = arrondir(remain.y);
    if(moveY != 0){
        remain.y -= moveY;
        int moveSign = signInt(moveY); // droite ou gauche
        bool collisionHappened = false;

        // bouge ent jusqu'à une collision
        while(!collisionHappened && moveY != 0){
            ent->physicsBox.y += moveSign;

            IntRectangle block;
            int centerBlock = findBlockMap(*ent, mapSizeX, mapSizeY);
            for (int i = -1; i<2; i++){
                for (int j = -1; j<2; j++){
                    int n = centerBlock + mapSizeY*i + j;
                    if (map[n] != 0){ // 0 = air
                        indexToIntRectangle(n, &block);
                        if (rectangleCollision(ent->physicsBox, block)){
                            // Moving down/falling
                            if(ent->speed.y > 0.0f){
                                grounded = true;
                            }
                            ent->speed.y = 0;
                            collisionHappened = true;
                            break;
	        		    }
	        	    }
                    if (collisionHappened) break;
                }
                if (collisionHappened) break;
            }
            if (collisionHappened) break;
            
            // bouge si pas de collision
            ent->position.y += moveSign;
            moveY -= moveSign;
        }
    }
}


void mouvement(Entity *player, int map[], float dt){

    // regarde la bonne direction
    if (player->speed.x > 0){
        player->direction = 1;
    }
    if (player->speed.x < 0){
        player->direction = -1;
    }

    // jump
    if(IsKeyPressed(KEY_UP) && grounded){
        player->speed.y = jumpSpeed;
        //player->speed.x += player->solidSpeed.x; // si plateformes qui bouge ?
        //player->speed.y += player->solidSpeed.y;
        //play_sound("jump");
        grounded = false;
    }

    if(!grounded){
        //player->animationState = PLAYER_ANIM_JUMP;
    }
    

    if (IsKeyDown(KEY_LEFT)){
        if (grounded){
            //player->animationState = PLAYER_ANIM_JUMP;
        }

        float mult = 1.0f;
        if(player->speed.x > 0.0f){
            mult = 3.0f; // pour tourner vite
        }
        //player->runAnimTime += dt;
        player->speed.x = approach(player->speed.x, -runSpeed, mult * runAcceleration * dt);
    }

    if (IsKeyDown(KEY_RIGHT)){
        if (grounded){
            //player->animationState = PLAYER_ANIM_JUMP;
        }

        float mult = 1.0f;
        if(player->speed.x < 0.0f){
            mult = 3.0f; // pour tourner vite
        }
        //player->runAnimTime += dt;
        player->speed.x = approach(player->speed.x, runSpeed, mult * runAcceleration * dt);
    }

    // friction
    if(!IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)){
        if(grounded){
            player->speed.x = approach(player->speed.x, 0, runReduce * dt);
        }
        else{
            player->speed.x = approach(player->speed.x, 0, flyReduce * dt);
        }
    }

    // gravitée
    player->speed.y = approach(player->speed.y, fallSpeed, gravity * dt);

    moveX(&(*player), map); // à vérifier
    moveY(&(*player), map);
}