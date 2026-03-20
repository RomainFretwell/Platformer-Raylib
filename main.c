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

// ----------------------------------------------------------------------------------------
//                                   Game functions
// ----------------------------------------------------------------------------------------

void LoadAllTextures(){
    Texture_background_0 = LoadTexture("resources/background_0.png");
    Texture_Block_Atlas = LoadTexture("resources/Block_Atlas.png");
    Texture_Player_Idle = LoadTexture("resources/Player_Idle.png");
    Texture_Hologram_Saw = LoadTexture("resources/Hologram_Saw.png");
    Texture_bow = LoadTexture("resources/bow.png");
    Texture_arrow = LoadTexture("resources/arrow.png");
    Texture_green_slime_idle = LoadTexture("resources/green_slime_idle.png");
}

void UnloadAllTextures(){
    UnloadTexture(Texture_background_0);
    UnloadTexture(Texture_Block_Atlas);
    UnloadTexture(Texture_Player_Idle);
    UnloadTexture(Texture_Hologram_Saw);
    UnloadTexture(Texture_bow);
    UnloadTexture(Texture_arrow);
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
        // TODO : set window position to a fix point
    }
    screenRatio = currentScreenSize.x / smallScreenSize.x;
}

void initSawTimers(Saw* saw){
    for (int i = 0; i<saw->nbMoves; i++){
        float d = distance(saw->posTab[i%saw->nbMoves], saw->posTab[(i+1)%saw->nbMoves]);
        saw->timeTab[i] = (Timer){d/saw->speed, 0.0f};
    }
}

void moveSaw(Saw* saw){
    updateTimer(&saw->timeTab[saw->moveState]);
    if (timerIsDone(&saw->timeTab[saw->moveState])){
        saw->moveState = (saw->moveState + 1)%saw->nbMoves;
        startTimer(&saw->timeTab[saw->moveState]);
    }
    int ordre = 2;
    saw->position.x = tweenSmooth(saw->timeTab[saw->moveState].lifetime - saw->timeTab[saw->moveState].timeleft, saw->timeTab[saw->moveState].lifetime, saw->posTab[(saw->moveState)%saw->nbMoves].x, saw->posTab[(saw->moveState + 1)%saw->nbMoves].x, ordre);
    saw->position.y = tweenSmooth(saw->timeTab[saw->moveState].lifetime - saw->timeTab[saw->moveState].timeleft, saw->timeTab[saw->moveState].lifetime, saw->posTab[(saw->moveState)%saw->nbMoves].y, saw->posTab[(saw->moveState + 1)%saw->nbMoves].y, ordre);
}

void initSawCircleFormation(int nbSaw, Saw sawTab[nbSaw], IntVector2 pos, int rayon, int skip, int speed, Texture2D texture){
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
        .worldType = 0, // forêt ?
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
    
    float gravity = 13.0f; //0.3f

    // player initialization
    Entity player = (Entity){
        .position = (IntVector2){350, 150},
        .speed = (Vector2){0.0f, 0.0f},
        .angle = 0.0f,
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
        .health = player.maxHealth,
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
    
    // bow
    Entity bow = (Entity){
        .texture = Texture_bow,
        .position = (IntVector2){(currentScreenSize.x-bow.texture.width)/2, (currentScreenSize.y-bow.texture.height)/2},
        .speed = player.speed,
        .angle = 135.0f,
        .direction = RIGHT,
        .animation.origin = (IntVector2){12, 12},
    };
    
    // arrow
    Entity arrow = (Entity){
        .position =  bow.position, // à changer à player.position
        .speed = player.speed,
        .angle = 0.0f,
        .direction = RIGHT,
        .texture = Texture_arrow,
        .animation.origin = (IntVector2){2, 14},
    };
    
    // slime
    Entity slime = (Entity){
        .texture = Texture_green_slime_idle,
        .position = (IntVector2){330, 180},
        .speed = (Vector2){0.0f, 0.0f},
        .angle = 0.0f,
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
    int fireSpeed = 10;
    float VabsMax = 50.0f;
    bool grabbed = true;
    float testAngleArc = 1.0f;
    float acceleration = gravity;
    bool showCross = false; // pour dessinà supprimer
    bool showDebugInfo = false;

    // time variables
    double deltaTime = 0.005;
    int maxFPS = 200;
    SetTargetFPS(maxFPS);

    // ----------------------------------------------------------------------------------------
    //                                   Game loop
    // ----------------------------------------------------------------------------------------

    while (!WindowShouldClose()){

        // Toggle fullscreen
        if (IsKeyPressed(KEY_F11)){
            ToggleFullscreenWindow();

            // besoin de ça ??
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

        mouvement(&player, mapDeTest);
        updatePhysicsBoxEntity(&player);

        // ----------------------------------------------------------------------------------------
        //                                   Arrow
        // ----------------------------------------------------------------------------------------

        // test arc et flèche qui tourne
        testAngleArc = 0.1;
        bow.angle = bow.angle + testAngleArc;
        
        // grab arrow
        // TODO : changer distance arrow-player (pas arrow-arrow pour test)
        if (IsKeyPressed(KEY_E) && distance(arrow.position, arrow.position) < 50){
            grabbed = !grabbed;

            arrow.position = bow.position; // à modifier car décalage
            arrow.angle = bow.angle - 135;
            acceleration = gravity;
            arrow.speed = bow.speed;
        }

        if (grabbed){
            if (IsKeyPressed(KEY_Q)){ // clavier en qwerty ????
                grabbed = false;

                arrow.position = bow.position; // à modifier car décalage
                arrow.angle = bow.angle - 135;
                acceleration = gravity;
                arrow.speed.x = fireSpeed * cosf(arrow.angle * PI/180);
                arrow.speed.y = fireSpeed * sinf(arrow.angle * PI/180);
            }
            else {
                arrow.speed = bow.speed;
                arrow.position = bow.position; // à modifier car décalage
                arrow.angle = bow.angle - 135;
            }
        }

        else {          
            
            // gravité flèche
            arrow.speed.y += acceleration * deltaTime;
        
            // limite vitesse flèche
            if (distanceFloat(arrow.speed, (Vector2) {0.0f, 0.0f}) > VabsMax) {
                arrow.speed.x = VabsMax * cosf(arrow.angle * PI / 180);
                arrow.speed.y = VabsMax * sinf(arrow.angle * PI / 180);
            }
            
            // si collion avec block alors speed = 0 et acceleration = 0

            arrow.position.x += arrow.speed.x * screenRatio * deltaTime;
            arrow.position.y += arrow.speed.y * screenRatio * deltaTime;

            // arrow angle
            if (arrow.speed.x != 0 || arrow.speed.y != 0){
                if (arrow.speed.y == 0){
                    arrow.angle = 180 * (1-signe(arrow.speed.x))/2;
                }
                if (arrow.speed.x == 0){
                    arrow.angle = 90 * signe(arrow.speed.y);
                }
                else {
                    if (arrow.speed.x > 0){
                        arrow.angle = (int) (atan(arrow.speed.y / arrow.speed.x) * 180/PI);
                    }
                    else {
                        arrow.angle = 180 + (int) (atan(arrow.speed.y / arrow.speed.x) * 180/PI);
                    }
                }
            }

            // debut collision

            // simplifié
            // (arrow.position.y + arrow.texture.width*sinf(arrow.angle*PI/180)*180/PI)

            // réel
            // arrow.position.y + distance((Vector2){arrow.texture.width, arrow.texture.height/2}, (Vector2){0.0, 0.0})*sinf((arrow.angle + atan(arrow.texture.height/arrow.texture.width/2)
            if ((arrow.position.y + distanceFloat((Vector2){arrow.texture.width, arrow.texture.height/2}, (Vector2){0.0f, 0.0f})) >= currentScreenSize.y/screenRatio - 30){
                // arrow.position.y = currentScreenSize.y/screenRatio - 30;
                arrow.speed.x = 0.0f;
                arrow.speed.y = 0.0f;
                acceleration = 0.0f;
            }
        }
        
        // ----------------------------------------------------------------------------------------
        //                             update hitboxes
        // ----------------------------------------------------------------------------------------

        updateHitboxEntity(&player);
        updateHitboxEntity(&bow);
        updateHitboxEntity(&arrow);

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
        

        if (IsKeyPressed(KEY_P)){
            camera.zoom += 0.1;
        }
        if (IsKeyPressed(KEY_O)){
            camera.zoom -= 0.1;
        }
        
        limitCameraMap(&camera, mapDeTest);

        BeginMode2D(camera);

        if (!IsKeyDown(KEY_X)){
            cameraFollow(&camera, player);
            //cameraFollow2(&camera, player);
            //int cameraFollowThresh = 120;
            //limitCameraFollow(&camera, player, cameraFollowThresh);
        }

        Vector2 mouseWorldPos;
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            mouseWorldPos = GetMousePosition(); //(IntVector2){GetMouseX(), GetMouseY()};
            mouseWorldPos.x = (mouseWorldPos.x + camera.target.x - camera.offset.x) / (blockSize*screenRatio);
            mouseWorldPos.y = (mouseWorldPos.y + camera.target.y - camera.offset.y) / (blockSize*screenRatio);
            mapDeTest.data[ (int) mouseWorldPos.x  * mapDeTest.size.y + (int) mouseWorldPos.y ] = selectedMaterial;
            localAutoTile(mapDeTest, (int) mouseWorldPos.x, (int) mouseWorldPos.y);
        }
        else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
            mouseWorldPos = GetMousePosition(); //(IntVector2){GetMouseX(), GetMouseY()};
            mouseWorldPos.x = (mouseWorldPos.x + camera.target.x - camera.offset.x) / (blockSize*screenRatio);
            mouseWorldPos.y = (mouseWorldPos.y + camera.target.y - camera.offset.y) / (blockSize*screenRatio);
            mapDeTest.data[ (int) mouseWorldPos.x  * mapDeTest.size.y + (int) mouseWorldPos.y ] = 0;
            localAutoTile(mapDeTest, (int) mouseWorldPos.x, (int) mouseWorldPos.y);
        }
        
        drawMap(mapDeTest);
        //* test affichage map dans le terminal
        if (IsKeyPressed(KEY_T)){
            printf("\nmap =\n");
            for (int j = 0; j<mapDeTest.size.y; j++){
                for (int i = 0; i<20; i++){
                    printf("%d", mapDeTest.tiled[i*mapDeTest.size.y+j]);
                }
                printf("\n");
            }
            printf("\n");
        }
        //*/

        // affichage des entity (dans le bon ordre)
        drawEntity(player);
        //drawEntity(bow);
        //drawEntity(arrow);
        drawEntity(slime);

        moveSaw(&saw);
        DrawTextureEx(saw.texture, (Vector2){saw.position.x * screenRatio, saw.position.y * screenRatio}, 0, screenRatio, WHITE);

        // test scie en étoile
        for (int i=0; i<nbSawTest; i++){
            moveSaw(&tabSawTest[i]);
            DrawTextureEx(Texture_Hologram_Saw, (Vector2){tabSawTest[i].position.x * screenRatio, tabSawTest[i].position.y * screenRatio}, 0, screenRatio, WHITE);
        }
        
        
        if (showEntityHitbox){
            //drawHitbox(player.hitbox, RED);
            //drawHitbox(arrow.hitbox, ORANGE);
            //drawHitbox(bow.hitbox, YELLOW);
            
            DrawRectangleLines( player.physicsBox.x * screenRatio,
                                player.physicsBox.y * screenRatio,
                                player.physicsBox.width * screenRatio,
                                player.physicsBox.height * screenRatio, GREEN);
        }

        // test croix player
        if (showCross){
            drawCross(player.position.x, player.position.y, WHITE);
        }

        EndMode2D();


        // ----------------------------------------------------------------------------------------
        //                                 DEBUG INFO
        // ----------------------------------------------------------------------------------------

        if (showEntityHitbox){
            const char * test0 = TextFormat("entity hitbox ON");
            DrawText(test0, currentScreenSize.x - 180, 10, 20, RED);
        }

        // test player
        if (showDebugInfo){
            DrawFPS(10, 10);
            const char * test1 = TextFormat("World position X = %d", player.position.x);
            DrawText(test1, 10, 50, 20, WHITE);
            const char * test2 = TextFormat("World position Y = %d", player.position.y);
            DrawText(test2, 10, 70, 20, WHITE);
            Vector2 testScreen = GetWorldToScreen2D((Vector2) {player.position.x, player.position.y}, camera);
            const char * test01 = TextFormat("Screen position X = %f", testScreen.x);
            DrawText(test01, 10, 90, 20, WHITE);
            const char * test02 = TextFormat("Screen position Y = %f", testScreen.y);
            DrawText(test02, 10, 110, 20, WHITE);
            const char * test3 = TextFormat("speed X = %f", player.speed.x);
            DrawText(test3, 10, 130, 20, WHITE);
            const char * test4 = TextFormat("speed Y = %f", player.speed.y);
            DrawText(test4, 10, 150, 20, WHITE);
        }
        
        // test mouse
        const char * test5 = TextFormat("mouse SCREEN\nX = %f\nY = %f", GetMousePosition().x/screenRatio, GetMousePosition().y/screenRatio);
        DrawText(test5, 400, 50, 20, WHITE);
        const char * test6 = TextFormat("camera target\nX = %f\nY = %f", camera.target.x, camera.target.y);
        DrawText(test6, 400, 150, 20, WHITE);
        //const char * test7 = TextFormat("camera offset\nX = %f\nY = %f", camera.offset.x, camera.offset.y);
        //DrawText(test7, 400, 250, 20, WHITE);

        /* test arrow
        // test arrow
        const char * test8 = TextFormat("Arrow speed X = %f", arrow.speed.x);
        DrawText(test8, 500, 50, 20, WHITE);
        const char * test9 = TextFormat("Arrow speed Y = %f", arrow.speed.y);
        DrawText(test9, 500, 70, 20, WHITE);
        const char * test10 = TextFormat("Arrow position X = %d", arrow.position.x);
        DrawText(test10, 500, 90, 20, WHITE);
        const char * test11 = TextFormat("Arrow position Y = %d", arrow.position.y);
        DrawText(test11, 500, 110, 20, WHITE);
        const char * test12 = TextFormat("Arrow angle = %d", arrow.angle);
        DrawText(test12, 500, 130, 20, WHITE);
        const char * test15 = TextFormat("Bow speed Y = %f", bow.speed.y);
        DrawText(test15, 500, 150, 20, WHITE);
        */
        
        
        EndDrawing();

        deltaTime = GetFrameTime();
    }
    
    free(mapDeTest.data);
    free(mapDeTest.tiled);

    UnloadAllTextures();

    CloseWindow();

    return 0;
}