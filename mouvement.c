#include "mouvement.h"

const float runSpeed = 2.1f; // à 2.0f
const float runAcceleration = 10.0f;

const float runReduce = 22.0f; 
const float flyReduce = 12.0f;    

const float regularGravity = 13.0f;
float gravity = regularGravity;

const float fallSpeed = 3.8f;
const float jumpSpeed = -3.3f;
const float glideSpeed = 0.7f;
const float wallSlideSpeed = 0.3f;
const Vector2 wallJumpSpeed = {-2.3f, -3.0f};

bool wallSliding = false;
bool canGlide = true;

const double coyoteTime = 0.15;
double coyoteTimeCounter = coyoteTime;

const double jumpBuffer = 0.1;
double jumpBufferCounter = jumpBuffer;

const float jumpHangThresh = 0.2f;
const float hangGravity = regularGravity / 2;
const float hangBoostThresh = runSpeed * 0.1;
const float hangBoost = runSpeed * 1.2;


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
                            ent->speed.y = 0.0f;
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

bool isWallSliding(Entity *player, int map[]){
    if (player->grounded) return false;
    int touchWall = 2;
    if (touchWall < player->physicsBox.x - blockSize * (int) (player->physicsBox.x / blockSize) && 
        touchWall < blockSize * (int) ((player->physicsBox.x + player->physicsBox.width) / blockSize) - (player->physicsBox.x + player->physicsBox.width))
        return false;

    int sideBlock = findBlockMap(*player, mapSizeX, mapSizeY) + mapSizeY*player->direction;
    if (map[sideBlock + mapSizeY + 1] != 0 || map[sideBlock - 2*mapSizeY*player->direction + 1] != 0){
        return false;
    }
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

    // coyote jump
    if(player->grounded || wallSliding){
        coyoteTimeCounter = coyoteTime;
    }
    else {
        coyoteTimeCounter -= dt;
        //player->animationState = PLAYER_ANIM_JUMP;
    }

    if (IsKeyDown(KEY_UP)){
        jumpBufferCounter = jumpBuffer;
    }
    else {
        jumpBufferCounter -= dt;
    }
    
    // glide
    if (player->grounded || wallSliding){
        canGlide = false;
    }
    else if (!IsKeyDown(KEY_UP) && player->speed.y > 0){
        canGlide = true;
    }

    // slide against wall
    wallSliding = isWallSliding(&(*player), map);

    if (IsKeyDown(KEY_DOWN)){
        wallSliding = false;
    }
    
    const char * test1 = TextFormat("%s", wallSliding?"sliding":"");
    DrawText(test1, 100*screenRatio, 250*screenRatio, 10*screenRatio, BLACK);

    const char * test2 = TextFormat("%s", player->grounded?"grounded":"");
    DrawText(test2, 100*screenRatio, 260*screenRatio, 10*screenRatio, BLACK);

    const char * test3 = TextFormat("coyoteTimeCounter = %f", coyoteTimeCounter);
    DrawText(test3, 100*screenRatio, 270*screenRatio, 10*screenRatio, BLACK);

    const char * test4 = TextFormat("jumpBufferCounter = %f", jumpBufferCounter);
    DrawText(test4, 100*screenRatio, 280*screenRatio, 10*screenRatio, BLACK);

    // jump
    if (coyoteTimeCounter > 0.0 && jumpBufferCounter > 0.0 && !wallSliding){
        player->speed.y = jumpSpeed;
        //player->speed.x += player->solidSpeed.x; // si plateformes qui bouge ?
        //player->speed.y += player->solidSpeed.y;
        //play_sound("jump");
        coyoteTimeCounter = 0.0;
        jumpBufferCounter = 0.0;
        player->grounded = false;
    }
    else if(IsKeyPressed(KEY_UP) && wallSliding){
        player->speed.y = wallJumpSpeed.y;
        player->speed.x = player->direction * wallJumpSpeed.x;
        //player->speed.x += player->solidSpeed.x; // si plateformes qui bouge ?
        //player->speed.y += player->solidSpeed.y;
        //play_sound("jump");
        coyoteTimeCounter = 0.0;
        jumpBufferCounter = 0.0;
        player->grounded = false;
    }

    // controler la hauteur du saut en appuyant plus ou moins longtemps
    if (!IsKeyDown(KEY_UP) && player->speed.y < -0.1f){
        player->speed.y *= 0.5f;
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
        //if (wallSliding){
        //    player->speed.x = wallJumpSpeed.x;
        //}
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
        //if (wallSliding){
        //    player->speed.x = - wallJumpSpeed.x;
        //}
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

    // better jump hang (moins de gravité au sommet + boost de vitesse)
    if (absf(player->speed.y) < jumpHangThresh && !player->grounded){
        //const char * test5 = TextFormat("jump hang");
        //DrawText(test5, 200*screenRatio, 200*screenRatio, 10*screenRatio, BLACK);
        gravity = hangGravity;
        if (absf(player->speed.x) > hangBoostThresh){
            player->speed.x = hangBoost * player->direction;
        }
    }
    else {
        gravity = regularGravity;
    }

    // glide
    if (IsKeyDown(KEY_UP) && canGlide){
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
    
    moveX(&(*player), map);
    updatePhysicsBoxEntity(&(*player));
    
    moveY(&(*player), map);
    updatePhysicsBoxEntity(&(*player));
    
    if (player->grounded){
        player->speed.y = 0;
    }
}