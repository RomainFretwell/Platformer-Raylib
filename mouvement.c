#include "mouvement.h"

static Vector2 remainder = {};
static bool grounded = false;
const float runSpeed = 2.0f;
const float runAcceleration = 10.0f;
const float runReduce = 22.0f; 
const float flyReduce = 12.0f;    
const float gravity = 13.0f;
const float fallSpeed = 3.6f;
const float jumpSpeed = -3.0f;

// modifier pour utiliser pour tout les mob ???
void mouvement(Entity player, float dt){ // passer par adresse Entity *
    
    // face correct direction
    if (player.speed.x > 0){
        player.direction = 1;
    }
    if (player.speed.x < 0){
        player.direction = -1;
    }

    /*/ Jump
    if(IsKeyPressed(KEY_UP) && grounded){
        player.speed.y = jumpSpeed;
        player.speed.x += player.solidSpeed.x;
        player.speed.y += player.solidSpeed.y;
        play_sound("jump");
        grounded = false;
    }

    if(!grounded){
        player.animationState = PLAYER_ANIM_JUMP;
    }
    */

    if (IsKeyDown(KEY_LEFT)){
        /*if (grounded){
            player.animationState = PLAYER_ANIM_JUMP;
        }
        */

        float mult = 1.0f;
        if(player.speed.x > 0.0f){
            mult = 3.0f;
        }
        //player.runAnimTime += dt;
        player.speed.x = approach(player.speed.x, -runSpeed, runAcceleration * mult * dt);
    }

    if (IsKeyDown(KEY_RIGHT)){
        /*if (grounded){
            player.animationState = PLAYER_ANIM_JUMP;
        }
        */

        float mult = 1.0f;
        if(player.speed.x < 0.0f){
            mult = 3.0f;
        }
        //player.runAnimTime += dt;
        player.speed.x = approach(player.speed.x, runSpeed, runAcceleration * mult * dt);
    }

    // Friction
    if(!IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)){
        if(grounded){
            player.speed.x = approach(player.speed.x, 0, runReduce * dt);
        }
        else{
            player.speed.x = approach(player.speed.x, 0, flyReduce * dt);
        }
    }

    // Gravity
    player.speed.y = approach(player.speed.y, fallSpeed, gravity * dt);


    


    // #########   pour X  ############

    IntRectangle playerRect; // remplacer playerRect par hitbox
    playerRect.x = player.hitbox.x;
    playerRect.y = player.hitbox.y;
    playerRect.width = player.hitbox.width;
    playerRect.height = player.hitbox.height;

    remainder.x += player.speed.x;
    int moveX = round(remainder.x);
    if(moveX != 0){
        remainder.x -= moveX;
        int moveSign = signInt(moveX);
        bool collisionHappened = false;








        // ----------------   changer et utiliser handleCollision()   ------------------

        // Move the player in Y until collision or moveY is exausted
        while(moveX){
            playerRect.position.x += moveSign;

            // Test collision against Solids
            for(int solidIdx = 0; solidIdx < gameState->solids.count; solidIdx++){
                Solid& solid = gameState->solids[solidIdx];
                IntRectangle solidRect = get_solid_rect(solid);

                if(rect_collision(playerRect, solidRect)){
                    player.speed.x = 0;
                    return; // à remplacer par moveX = 1 pour sortir ? (car avant c'était une lambda function)
                }
            }

            // Loop through LOCAL Tiles -> comme fonction handleCollisions
            IntVector2 playerGridPos = get_grid_pos(player.position);
            for(int x = playerGridPos.x - 1; x <= playerGridPos.x + 1; x++){
                for(int y = playerGridPos.y - 2; y <= playerGridPos.y + 2; y++){
                    Tile* tile = get_tile(x, y);

                    if(!tile || !tile->isVisible){
                        continue;
                    }

                    IntRectangle tileRect = get_tile_rect(x, y);
                    if(rect_collision(playerRect, tileRect)){
                        player.speed.x = 0;
                        return;
                    }
                }
            }

            // Move the Player if no collisions
            player.position.x += moveSign;
            moveX -= moveSign;
        }
        
    }


    // #########   pour Y   ############   (copier-coller mais jump et gravité)



}