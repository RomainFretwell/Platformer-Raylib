#include "mouvement.h"

const float runSpeed = 2.1f; // à 2.0f
const float runAcceleration = 10.0f;
const float runReduce = 22.0f; 
const float flyReduce = 12.0f;    
const float gravity = 13.0f;
const float fallSpeed = 3.6f;
const float jumpSpeed = -3.0f;
const float glideSpeed = 0.7f;
const float wallSlideSpeed = 0.3f;
bool wallSliding = false;

/*// à faire plus tard
bool isGrounded(Entity ent, int map[]){
    //return ||  ||  || ;
}

bool isSliding(Entity ent, int map[]){
    //return ||  ||  || ;
}
*/

static void moveX(Entity *ent, int map[]){
    ent->remain.x += ent->speed.x;
    int moveX = arrondir(ent->remain.x);
    
    if(moveX != 0){
        ent->remain.x -= moveX;
        int moveSign = signInt(moveX); // droite ou gauche
        bool collisionHappened = false;

        int centerBlock = findBlockMap(*ent, mapSizeX, mapSizeY);

        // bouge ent jusqu'à une collision
        while(!collisionHappened && moveX != 0){
            ent->physicsBox.x += moveSign;

            IntRectangle block;
            for (int i = -1; i<2; i++){
                for (int j = -1; j<2; j++){
                    int n = centerBlock + mapSizeY*i + j;
                    if (map[n] != 0){ // 0 = air
                        indexToIntRectangle(n, &block);
                        if (rectangleCollision(ent->physicsBox, block)){
                            ent->speed.x = 0;
                            ent->remain.x = 0.0f;
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
    ent->remain.y += ent->speed.y;
    int moveY = arrondir(ent->remain.y);
    if(moveY != 0){
        ent->remain.y -= moveY;
        int moveSign = signInt(moveY); // droite ou gauche
        bool collisionHappened = false;

        int centerBlock = findBlockMap(*ent, mapSizeX, mapSizeY);

        // bouge ent jusqu'à une collision
        while(!collisionHappened && moveY != 0){
            ent->physicsBox.y += moveSign;

            IntRectangle block;
            for (int i = -1; i<2; i++){
                for (int j = -1; j<2; j++){
                    int n = centerBlock + mapSizeY*i + j;
                    if (map[n] != 0){ // 0 = air
                        indexToIntRectangle(n, &block);
                        if (rectangleCollision(ent->physicsBox, block)){
                            if(ent->speed.y > 0.0f){ // falling
                                ent->grounded = true;
                            }
                            ent->speed.y = 0;
                            ent->remain.y = 0.0f;
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
            ent->grounded = false;
        }
    }
}

bool isWallSliding(Entity *ent, int map[]){
    if (ent->grounded) return false;
    int touchWall = 2;
    if (touchWall < ent->physicsBox.x - blockSize * (int) (ent->physicsBox.x / blockSize) && 
        touchWall < blockSize * (int) ((ent->physicsBox.x + ent->physicsBox.width) / blockSize) + blockSize - (ent->physicsBox.x + ent->physicsBox.width))
        return false;

    int sideBlock = findBlockMap(*ent, mapSizeX, mapSizeY) + mapSizeY*ent->direction;
    if (map[sideBlock-1] != 0 || map[sideBlock] != 0 || map[sideBlock+1] != 0){
        return true;
    }
    return false;
}

void mouvement(Entity *player, int map[], float dt){

    // regarde la bonne direction
    if (player->speed.x > 0){
        player->direction = 1;
    }
    if (player->speed.x < 0){
        player->direction = -1;
    }

    
    // slide against wall
    wallSliding = isWallSliding(&(*player), map);
    const char * test = TextFormat("wallSliding = %s", wallSliding?"oui":"non");
    DrawText(test, 400, 30, 20, BLACK);

    // jump
    if(IsKeyPressed(KEY_UP)){
        if (player->grounded){
            player->speed.y = jumpSpeed;
            //player->speed.x += player->solidSpeed.x; // si plateformes qui bouge ?
            //player->speed.y += player->solidSpeed.y;
            //play_sound("jump");
            player->grounded = false;
        }
        else if(wallSliding){
            player->speed.y = jumpSpeed;
            player->speed.x = - player->direction * runSpeed;
            //player->speed.x += player->solidSpeed.x; // si plateformes qui bouge ?
            //player->speed.y += player->solidSpeed.y;
            //play_sound("jump");
            player->grounded = false;
        }
    }

    // controler la hauteur du saut en appuyant plus ou moins longtemps
    if (!IsKeyDown(KEY_UP) && player->speed.y < 0){
        player->speed.y = 0;
    }

    if(!player->grounded){
        //player->animationState = PLAYER_ANIM_JUMP;
    }
    

    if (IsKeyDown(KEY_LEFT)){
        if (player->grounded){
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
        if (player->grounded){
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
        if(player->grounded){
            player->speed.x = approach(player->speed.x, 0, runReduce * dt);
        }
        else{
            player->speed.x = approach(player->speed.x, 0, flyReduce * dt);
        }
    }

    // glide
    if (IsKeyDown(KEY_G) && player->speed.y > 0){
        player->speed.y = approach(player->speed.y, glideSpeed, gravity * dt);
        // animation gliding
    }
    else if (wallSliding){
        player->speed.y = approach(player->speed.y, wallSlideSpeed, gravity * dt);
        // animation sliding selon direction
    }
    else { // gravitée
        player->speed.y = approach(player->speed.y, fallSpeed, gravity * dt);
    }
    
    moveX(&(*player), map); // à vérifier
    updatePhysicsBoxEntity(&(*player));
    
    moveY(&(*player), map);
    updatePhysicsBoxEntity(&(*player));
}