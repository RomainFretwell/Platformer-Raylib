// ----------------------------------------------------------------------------------------
//                                   Platformer in raylib
//                                  made by Romain Fretwell
// ----------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "structures.h"
#include "global.h"
#include "math2.h"
#include "map.h"
#include "draw.h"
#include "animation.h"
#include "camera.h"
#include "collision.h"
#include "mouvement.h"
#include "saw.h"


// ----------------------------------------------------------------------------------------
//                                   Game functions
// ----------------------------------------------------------------------------------------

void LoadAllTextures(){
    Texture_background_0 = LoadTexture("resources/background_0.png");
    Texture_Block_Atlas = LoadTexture("resources/Block_Atlas.png");
    Texture_Player_Idle = LoadTexture("resources/Player_Idle.png");
    Texture_Hologram_Saw = LoadTexture("resources/Hologram_Saw.png");
    Texture_green_slime_idle = LoadTexture("resources/green_slime_idle.png");
}

void UnloadAllTextures(){
    UnloadTexture(Texture_background_0);
    UnloadTexture(Texture_Block_Atlas);
    UnloadTexture(Texture_Player_Idle);
    UnloadTexture(Texture_Hologram_Saw);
    UnloadTexture(Texture_green_slime_idle);
}

void ToggleFullscreenWindow(){
    if (!IsWindowFullscreen()){
        SetWindowSize(fullScreenSize.x, fullScreenSize.y);
        ToggleFullscreen();
        currentScreenSize.x = fullScreenSize.x;
        currentScreenSize.y = fullScreenSize.y;
    } else {
        ToggleFullscreen();
        SetWindowSize(smallScreenSize.x, smallScreenSize.y);
        currentScreenSize.x = smallScreenSize.x;
        currentScreenSize.y = smallScreenSize.y;
    }
    screenRatio = currentScreenSize.x / smallScreenSize.x;
}

void mapInTerminal(Map map){
    printf("\n       ---------- map ----------  \n");
    for (int j = 0; j<map.size.y; j++){
        for (int i = 0; i<140; i++){ // peut pas tout mettre sans retour à la ligne
            printf("%d", map.data[i*map.size.y+j]);
        }
        printf("\n");
    }
    printf("\n");
}

// ----------------------------------------------------------------------------------------
//
//                                   Main function
//
// ----------------------------------------------------------------------------------------

int main(){
    
    // Window initialization
    InitWindow(smallScreenSize.x, smallScreenSize.y, "jeu 2D avec Raylib");
    fullScreenSize.x = GetMonitorWidth(GetCurrentMonitor());
    fullScreenSize.y = GetMonitorHeight(GetCurrentMonitor());
    currentScreenSize = smallScreenSize;
    screenRatio = 1.0f;
    
    LoadAllTextures();
    
    // Map initialization
    Map mapDeTest = (Map){
        .worldType = 0,
        .size.x = 150,
        .size.y = 70,
        .tileSet.texture = Texture_Block_Atlas,
        .tileSet.size = (IntVector2){20, 1},
        .data = malloc(sizeof(int) * mapDeTest.size.x * mapDeTest.size.y),
        .tiled = malloc(sizeof(int) * mapDeTest.size.x * mapDeTest.size.y),
    };
    clearMap(mapDeTest);
    testMap(mapDeTest);
    completeAutoTile(mapDeTest);
    
    int selectedMaterial = BLUE_BRICKS;

    // player initialization
    Entity player = (Entity){
        .position = (IntVector2){350, 150},
        .speed = (Vector2){0.0f, 0.0f},
        .direction = RIGHT,
        .texture = Texture_Player_Idle,

        // physics box
        .physicsBox.width = 14,
        .physicsBox.height = 32,
        .physicsBox.x = player.position.x - player.physicsBox.width/2,
        .physicsBox.y = player.position.y - player.physicsBox.height/2,
        .remain = (Vector2){0.0f,0.0f},
        .grounded = false,

        // animation
        .animState = IDLE,
        .animation.type = REPEATING,
        .animation.first = 0,
        .animation.last = 10, // nbFrames - 1
        .animation.current = player.animation.first,
        .animation.step = 1,
        .animation.frameSize = (IntVector2){64,48},
        .animation.origin = (IntVector2){32, 24},
        .animation.timer = (Timer){0.15f, 0.0f},

        .maxHealth = 100,
        .health = 80,
    };

    // saw initialization
    Saw saw = (Saw){
        .position = (IntVector2){500, 200},
        .nbMoves = 4,
        .moveState = 0,
        .speed = 300, // à changer

        .hitbox = (Rectangle){338, 238, 24, 24},
        .damage = 5,

        .texture = Texture_Hologram_Saw,

        // animation
        .animState = IDLE,
        .animation.type = REPEATING,
        .animation.first = 0,
        .animation.last = 0, // nbFrames - 1
        .animation.current = saw.animation.first,
        .animation.step = 1,
        .animation.frameSize = (IntVector2){32,32},
        .animation.origin = (IntVector2){16, 16},
        .animation.timer = (Timer){0.15f, 0.0f},
    };

    saw.posTab[0] = saw.position;
    saw.posTab[1] = (IntVector2){saw.position.x + 400, saw.position.y};
    saw.posTab[2] = (IntVector2){saw.position.x + 400, saw.position.y - 100};
    saw.posTab[3] = (IntVector2){saw.position.x + 200, saw.position.y};
    
    initSawTimers(&saw);

    int nbSawTest = 5;
    Saw tabSawTest[nbSawTest];
    initSawCircleFormation(nbSawTest, tabSawTest, (IntVector2){800, 700}, 300, 2, 200, Texture_Hologram_Saw);
    
    // slime
    Entity slime = (Entity){
        .texture = Texture_green_slime_idle,
        .position = (IntVector2){330, 180},
        .speed = (Vector2){0.0f, 0.0f},
        .direction = LEFT,

        // physics box
        .physicsBox.width = 14,
        .physicsBox.height = 8,
        .physicsBox.x = player.position.x - player.physicsBox.width/2,
        .physicsBox.y = player.position.y - player.physicsBox.height/2,
        .remain = (Vector2){0.0f,0.0f},
        .grounded = false,
        
        // animation
        .animState = IDLE,
        .animation.type = REPEATING,
        .animation.first = 0,
        .animation.last = 5, // nbFrames - 1
        .animation.current = slime.animation.first,
        .animation.step = 1,
        .animation.frameSize = (IntVector2){16, 16},
        .animation.origin = (IntVector2){8, 9},
        .animation.timer = (Timer){0.15f, 0.0f},
    };

    // camera
    Camera2D camera = {
        .target = (Vector2){player.position.x * screenRatio, player.position.y * screenRatio},
        .offset = (Vector2){currentScreenSize.x/2, currentScreenSize.y/2},
        .rotation = 0,
        .zoom = 1
    };
    
    // autres variables
    bool showCross = false;
    bool showDebugInfo = false;

    // time variables
    deltaTime = 0.005f;
    int maxFPS = 200;
    SetTargetFPS(maxFPS);

    // ----------------------------------------------------------------------------------------
    //                                   Game loop
    // ----------------------------------------------------------------------------------------

    while (!WindowShouldClose()){

        // Toggle fullscreen
        if (IsKeyPressed(KEY_F11)){
            ToggleFullscreenWindow();
            camera.offset = (Vector2){currentScreenSize.x/2, currentScreenSize.y/2};
            camera.target = (Vector2){player.position.x * screenRatio, player.position.y * screenRatio};
        }
        if (IsKeyPressed(KEY_F3)){
            showDebugInfo = !showDebugInfo;
        }
        if (IsKeyPressed(KEY_H)){
            showBlockHitbox = !showBlockHitbox;
            showEntityHitbox = !showEntityHitbox;
        }
        if (IsKeyPressed(KEY_C)){
            showCross = !showCross;
        }
        if (IsKeyPressed(KEY_T)){
            mapInTerminal(mapDeTest);
        }
        if (IsKeyPressed(KEY_P)){
            camera.zoom += 0.1;
        }
        if (IsKeyPressed(KEY_O)){
            camera.zoom -= 0.1;
        }
        if (IsKeyPressed(KEY_L)){
            player.health -= 5;
        }
        if (IsKeyPressed(KEY_SEMICOLON)){ // M en azerty
            player.health += 5;
        }
        

        // ----------------------------------------------------------------------------------------
        //                             update positions
        // ----------------------------------------------------------------------------------------

        mouvement(&player, mapDeTest);
        updatePhysicsBoxEntity(&player);
        
        // ----------------------------------------------------------------------------------------
        //                             update animations
        // ----------------------------------------------------------------------------------------

        updateAnimation(&player.animation);
        updateAnimation(&slime.animation);
        updateAnimation(&saw.animation);
        for (int i=0; i<nbSawTest; i++){
            updateAnimation(&tabSawTest[i].animation);
        }

        // ----------------------------------------------------------------------------------------
        //                                   Drawing
        // ----------------------------------------------------------------------------------------
        
        BeginDrawing();
        
        ClearBackground((Color){32, 36, 85, 255});
        DrawTextureEx(Texture_background_0, (Vector2){0, 0}, 0, screenRatio, WHITE);
        
        BeginMode2D(camera);

        if (!IsKeyDown(KEY_X)){
            //cameraFollow(&camera, player);
            
            //cameraFollow2(&camera, player);
            //int cameraFollowThresh = 120;
            //limitCameraFollow(&camera, player, cameraFollowThresh);
        }

        Vector2 lookAhead = cameraLookAhead(50, player);

        camera.target.x += 10 * (player.position.x*screenRatio - camera.target.x) * deltaTime;
        camera.target.y += 5 * (player.position.y*screenRatio - camera.target.y) * deltaTime;
        
        Vector2 maxCameraDist = { .x = 120, .y = 80};
        camera.target.x = constrainf(camera.target.x, (player.position.x - maxCameraDist.x)*screenRatio, (player.position.x + maxCameraDist.x)*screenRatio);
        camera.target.y = constrainf(camera.target.y, (player.position.y - maxCameraDist.y)*screenRatio, (player.position.y + maxCameraDist.y)*screenRatio);
        
        //player.position.x*screenRatio + lookAhead.x;
        //camera.target.y = player.position.y*screenRatio + lookAhead.y;
        limitCameraMap(&camera, mapDeTest);
        



        // level editor
        Vector2 mouseWorldPos;
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            mouseWorldPos = GetMousePosition();
            mouseWorldPos.x = (mouseWorldPos.x + camera.target.x - camera.offset.x) / (blockSize*screenRatio);
            mouseWorldPos.y = (mouseWorldPos.y + camera.target.y - camera.offset.y) / (blockSize*screenRatio);
            mapDeTest.data[ (int) mouseWorldPos.x  * mapDeTest.size.y + (int) mouseWorldPos.y ] = selectedMaterial;
            localAutoTile(mapDeTest, (int) mouseWorldPos.x, (int) mouseWorldPos.y);
        }
        else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
            mouseWorldPos = GetMousePosition();
            mouseWorldPos.x = (mouseWorldPos.x + camera.target.x - camera.offset.x) / (blockSize*screenRatio);
            mouseWorldPos.y = (mouseWorldPos.y + camera.target.y - camera.offset.y) / (blockSize*screenRatio);
            mapDeTest.data[ (int) mouseWorldPos.x  * mapDeTest.size.y + (int) mouseWorldPos.y ] = 0;
            localAutoTile(mapDeTest, (int) mouseWorldPos.x, (int) mouseWorldPos.y);
        }
        
        drawMap(mapDeTest);

        drawEntity(player);
        drawEntity(slime);

        moveSaw(&saw, 2.0f);
        DrawTextureEx(saw.texture, (Vector2){saw.position.x * screenRatio, saw.position.y * screenRatio}, 0, screenRatio, WHITE);

        // test scie en étoile
        for (int i=0; i<nbSawTest; i++){
            moveSaw(&tabSawTest[i], 2.0f);
            DrawTextureEx(Texture_Hologram_Saw, (Vector2){tabSawTest[i].position.x * screenRatio, tabSawTest[i].position.y * screenRatio}, 0, screenRatio, WHITE);
        }
        
        if (showEntityHitbox){
            DrawRectangleLines( player.physicsBox.x * screenRatio,
                                player.physicsBox.y * screenRatio,
                                player.physicsBox.width * screenRatio,
                                player.physicsBox.height * screenRatio, RED);
        }


        EndMode2D();

        if (showCross){
            drawCross(GetWorldToScreen2D((Vector2){player.position.x*screenRatio, player.position.y*screenRatio}, camera).x, GetWorldToScreen2D((Vector2){player.position.x*screenRatio, player.position.y*screenRatio}, camera).y, WHITE);
            drawCross(GetWorldToScreen2D(camera.target, camera).x, GetWorldToScreen2D(camera.target, camera).y, RED);
        }
        

        // affichage barre de vie
        DrawRectangleLinesEx((Rectangle){currentScreenSize.x - screenRatio*150, screenRatio*10, screenRatio*140, screenRatio*10}, screenRatio, WHITE);
        DrawRectangle(currentScreenSize.x - screenRatio*149, screenRatio*11, screenRatio*138*player.health/player.maxHealth, screenRatio*8, (Color){0, 140, 0, 200});
        const char * textPV = TextFormat("%d PV", player.health);
        DrawText(textPV, currentScreenSize.x - screenRatio*148, 12*screenRatio, 7*screenRatio, WHITE);
        
        
        // ----------------------------------------------------------------------------------------
        //                                 DEBUG INFO
        // ----------------------------------------------------------------------------------------

        if (showDebugInfo){
            DrawFPS(10, 10);
            if (showEntityHitbox){
                const char * test0 = TextFormat("entity hitbox ON");
                DrawText(test0, 110, 7, 21, RED);
            }
            int debugX = 10;
            int debugY = 35;
            int debugSpace = 8;
            int debugFontSize = screenRatio * 7;
            const char * test1 = TextFormat("World : player X = %d", player.position.x);
            DrawText(test1, debugX, debugY, debugFontSize, WHITE);
            debugY += screenRatio*debugSpace;
            const char * test2 = TextFormat("World : player Y = %d", player.position.y);
            DrawText(test2, debugX, debugY, debugFontSize, WHITE);
            debugY += screenRatio*debugSpace;
            const char * test3 = TextFormat("speed X = %f", player.speed.x);
            DrawText(test3, debugX, debugY, debugFontSize, WHITE);
            debugY += screenRatio*debugSpace;
            const char * test4 = TextFormat("speed Y = %f", player.speed.y);
            DrawText(test4, debugX, debugY, debugFontSize, WHITE);
            debugY += screenRatio*debugSpace;
            Vector2 testScreen = GetWorldToScreen2D((Vector2) {player.position.x * screenRatio, player.position.y * screenRatio}, camera);
            const char * test01 = TextFormat("Screen : player X = %f", testScreen.x);
            DrawText(test01, debugX, debugY, debugFontSize, WHITE);
            debugY += screenRatio*debugSpace;
            const char * test02 = TextFormat("Screen : player Y = %f", testScreen.y);
            DrawText(test02, debugX, debugY, debugFontSize, WHITE);
            debugY += screenRatio*debugSpace * 2;
            // test mouse
            const char * test03 = TextFormat("mouse SCREEN : X = %f  Y = %f", GetMousePosition().x/screenRatio, GetMousePosition().y/screenRatio);
            DrawText(test03, debugX, debugY, debugFontSize, WHITE);
            debugY += screenRatio*debugSpace;
            const char * test04 = TextFormat("camera target : X = %f  Y = %f", camera.target.x, camera.target.y);
            DrawText(test04, debugX, debugY, debugFontSize, WHITE);
            debugY += screenRatio*debugSpace;
            const char * test05 = TextFormat("camera offset : X = %f  Y = %f", camera.offset.x, camera.offset.y);
            DrawText(test05, debugX, debugY, debugFontSize, WHITE);
            debugY += screenRatio*debugSpace;
        }
        
        
        EndDrawing();

        deltaTime = GetFrameTime();
    }
    
    free(mapDeTest.data);
    free(mapDeTest.tiled);

    UnloadAllTextures();

    CloseWindow();

    return 0;
}