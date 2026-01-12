#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "structures.h"
#include "constants.h"
#include "draw.h"
#include "math2.h"
#include "camera.h"
#include "collision.h"

// ----------------------------------------------------------------------------------------
//                                   Game functions
// ----------------------------------------------------------------------------------------

void ToggleFullscreenWindow(){
    if (!IsWindowFullscreen()){
        SetWindowSize(fullScreenSize.width, fullScreenSize.height);
        ToggleFullscreen();
        currentScreenSize.width = fullScreenSize.width;
        currentScreenSize.height = fullScreenSize.height;
    } else {
        ToggleFullscreen();
        SetWindowSize(smallScreenSize.width, smallScreenSize.height);
        currentScreenSize.width = smallScreenSize.width;
        currentScreenSize.height = smallScreenSize.height;
        // TODO : set window position to a fix point
    }
    screenRatio = currentScreenSize.width / smallScreenSize.width;
}

void clearMap(int map[], int mapWidth, int mapHeight){
    for (int x = 0; x < mapWidth; x++){
        for (int y = 0; y < mapHeight; y++){
            map[x*mapHeight + y] = 0;
        }
    }
}

void initializeMap(int map[], int mapWidth, int mapHeight){
    for (int x = 0; x < mapWidth; x++){
        for (int y = 0; y < mapHeight; y++){
            map[x*mapHeight + y] = 0; // changer pour copier ou upload tableau d'un autre fichier (faire un dossier map)
        }
    }

    // map de test en attendant ...
    for (int x = 0; x < mapWidth; x++){
        map[x*mapHeight + mapHeight - 1] = 1;
        map[x*mapHeight + mapHeight - 2] = 2;
    }
    for (int x = 20; x < mapWidth; x++){
        map[x*mapHeight + mapHeight - 3] = 4;
    }
    for (int x = 25; x < mapWidth; x+=3){
        map[x*mapHeight + mapHeight - 4] = 3;
    }
    for (int x = 0; x < mapWidth; x++){
        map[x*mapHeight] = 1;
        map[x*mapHeight + 1] = 1;
    }
    for (int y = 0; y < mapHeight; y++){
        map[y] = 1;
        map[y + mapHeight] = 1;
        map[y + (mapWidth-1)*mapHeight] = 1;
        map[y + (mapWidth-2)*mapHeight] = 1;
    }

    for (int x = 20; x < 25; x++){
        map[x*mapHeight + 20] = 4;
        map[x*mapHeight + 24] = 4;
    }
    for (int y = 21; y < 24; y++){
        map[20*mapHeight + y] = 4;
        map[24*mapHeight + y] = 4;
    }
}

int canJump(Entity ent){
    return (ent.position.y >= currentScreenSize.height-(2*blockSize*sizeCoef + ent.texture.height)*screenRatio);
}


// remplacer par colision // && cooldown(variable, jumpCooldown)
// reset timer cooldown



// ----------------------------------------------------------------------------------------
//
//                                   Main function
//
// ----------------------------------------------------------------------------------------

int main(){
    
    // Window initialization
    InitWindow(smallScreenSize.width, smallScreenSize.height, "jeu 2D avec Raylib");
    fullScreenSize.width = GetMonitorWidth(GetCurrentMonitor());
    fullScreenSize.height = GetMonitorHeight(GetCurrentMonitor());
    currentScreenSize = smallScreenSize;
    screenRatio = 1.0;
    
    // Load block textures
    Block air;
    air.texture = LoadTexture("resources/air.png"); // transparent texture
    air.solid = false;
    air.breakable = false;
    air.coefRebond = 0.0;
    Block dirt;
    dirt.texture = LoadTexture("resources/dirt.png");
    dirt.solid = true;
    dirt.breakable = true;
    dirt.coefRebond = 0.0;
    Block grass;
    grass.texture = LoadTexture("resources/grass.png");
    grass.solid = true;
    grass.breakable = true;
    grass.coefRebond = 0.0;
    Block stone;
    stone.texture = LoadTexture("resources/stone.png");
    stone.solid = true;
    stone.breakable = true;
    stone.coefRebond = 0.0;
    Block gravel;
    gravel.texture = LoadTexture("resources/gravel.png");
    gravel.solid = true;
    gravel.breakable = true;
    gravel.coefRebond = 0.0;

    Block blockID[nbBlock] = {air, dirt, grass, stone, gravel};

    // Map initialization
    mapSizeX = 150;
    mapSizeY = 70;
    int map[mapSizeX * mapSizeY];
    clearMap(map, mapSizeX, mapSizeY);
    initializeMap(map, mapSizeX, mapSizeY);
    
    Color background_color = {220, 230, 255, 255};

    float gravity = 0.3;

    // player initialization
    Entity player;
    player.position = (Vector2){150, 200};
    player.VxMax = 5.0;
    player.VyMax = 10.0;
    player.speed = (Vector2){0, 0};
    player.acceleration = (Vector2){0.0, gravity};
    float acceleration_ground = 0.1;
    player.angle = 0;
    player.direction = 1;
    player.texture = LoadTexture("resources/druid.png");
    player.hitbox.bottomOffset = 2;
    player.hitbox.topOffset = 6;
    player.hitbox.leftOffset = 8;
    player.hitbox.rightOffset = 4;
    updateHitboxEntity(&player);
    
    // bow
    Entity bow;
    bow.texture = LoadTexture("resources/bow.png");
    bow.hitbox.bottomOffset = 0;
    bow.hitbox.topOffset = 0;
    bow.hitbox.leftOffset = 0;
    bow.hitbox.rightOffset = 0;
    bow.position = (Vector2){(currentScreenSize.width-bow.texture.width)/2, (currentScreenSize.height-bow.texture.height)/2};
    bow.speed = player.speed;
    bow.acceleration = player.acceleration;
    bow.angle = 135;
    bow.direction = 1;
    updateHitboxEntity(&bow);

    Entity arrow;
    arrow.position =  bow.position; // à changer à player.position
    arrow.speed = player.speed;
    arrow.acceleration = player.acceleration; // pas de gravité si pas tirée
    int fireSpeed = 10;
    arrow.VabsMax = 50.0;
    arrow.angle = 0;
    arrow.direction = 1;
    arrow.grabbed = true;
    arrow.texture = LoadTexture("resources/arrow.png");
    arrow.hitbox.bottomOffset = 0;
    arrow.hitbox.topOffset = 0;
    arrow.hitbox.leftOffset = 0;
    arrow.hitbox.rightOffset = 0;
    updateHitboxEntity(&arrow);
    
    // other variables
    float jumpCooldown = 1;
    float coefRebond = 0.1; // 0 = pas de rebond, 1 = rebond parfait
    float testAngleArc = 1;

    // camera
    Camera2D camera = {
        .target = (Vector2){player.position.x * screenRatio, player.position.y * screenRatio},
        .offset = (Vector2){currentScreenSize.width/2, currentScreenSize.height/2},
        .rotation = 0,
        .zoom = 1
    };
    
    // time variables
    double frameStart;
    double frameEnd;
    double deltaTime = 0.01;
    int size = 1;

    float constanteFPS = 80.0; // 80.0
    int maxFPS = 200;
    SetTargetFPS(maxFPS);


// ----------------------------------------------------------------------------------------
//                                   Game loop
// ----------------------------------------------------------------------------------------

    while (!WindowShouldClose()){
        
        frameStart = GetTime();
        
        // Toggle fullscreen
        if (IsKeyPressed(KEY_F11)){
            ToggleFullscreenWindow();
            camera.offset = (Vector2){currentScreenSize.width/2, currentScreenSize.height/2};
            camera.target = (Vector2){player.position.x * screenRatio, player.position.y * screenRatio};
        }
























//      ##################   Mouvement   #######################

        if (IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT) && player.speed.x < player.VxMax){
            player.speed.x += acceleration_ground;
            player.direction = 1;
        }
        else if (IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT) && player.speed.x > -player.VxMax){
            player.speed.x -= acceleration_ground;
            player.direction = -1;
        }
        // ralentissement friction sol
        else if (!IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)){
            if (player.speed.x > 0){
                player.speed.x -= acceleration_ground;
                if (player.speed.x < 0) player.speed.x = 0.0;
            }
            if (player.speed.x < 0){
                player.speed.x += acceleration_ground;
                if (player.speed.x > 0) player.speed.x = 0.0;
            }
            if (-0.1 < player.speed.x && player.speed.x < 0.1){
                player.speed.x = 0.0;
                player.acceleration.x = 0.0;
            }
        }

        if (IsKeyDown(KEY_SPACE)){
            player.position.y = 0;
            player.speed.y = 0;
            player.acceleration.y = gravity;
        }
        
        // test haut / bas
        if (IsKeyDown(KEY_UP)){
            player.speed.y = -5.0;
        }
        else if (IsKeyDown(KEY_DOWN)){
            player.speed.y = 5.0;
        }
        else {
            player.speed.y = 0.0;
        }

        /* jump + gravitée + collision sol
        // player jump
        if (IsKeyDown(KEY_UP) && canJump(player)){
            player.speed.y = - 2 * player.VyMax;
            player.acceleration.y = gravity;
            // jumpCooldown = 1;
        }

        // gravité player
        player.speed.y += player.acceleration.y * GetFrameTime() * constanteFPS;
        // limite vitesse player
        if (player.speed.x > player.VxMax) player.speed.x = player.VxMax;
        if (player.speed.y > player.VyMax) player.speed.y = player.VyMax;
        if (player.speed.x < -player.VxMax) player.speed.x = -player.VxMax;
        if (player.speed.y < -player.VyMax) player.speed.y = -player.VyMax;

        // Rebond
        if (player.position.y * screenRatio >= currentScreenSize.height-(2*blockSize*sizeCoef + player.texture.height)*screenRatio){
            player.position.y = currentScreenSize.height / screenRatio - (2*blockSize*sizeCoef + player.texture.height);
            if (player.speed.y <= 2.){
                player.speed.y = 0;
                player.acceleration.y = 0;
            }
            else if (jumpCooldown == 1){
                player.speed.y *= -coefRebond;
                player.acceleration.y = gravity;
            }
        }
        jumpCooldown -= 0.1 * GetFrameTime() * 200;

        */
        
        player.position.x += player.speed.x * GetFrameTime() * constanteFPS;
        player.position.y += player.speed.y * GetFrameTime() * constanteFPS;

        float totalSpeed = distance(player.speed, (Vector2){0, 0});
        
        

        if (IsKeyPressed(KEY_H)){
            showBlockHitbox = !showBlockHitbox;
            showEntityHitbox = !showEntityHitbox;
        }
        

// ----------------------------------------------------------------------------------------
//                                   Arrow
// ----------------------------------------------------------------------------------------

        // test arc et flèche qui tourne
        testAngleArc = 0.1;
        bow.angle = bow.angle + testAngleArc;
        
        // grab arrow
        // TODO : changer distance arrow-player (pas arrow-arrow pour test)
        if (IsKeyPressed(KEY_E) && distance(arrow.position, arrow.position) < 50){
            arrow.grabbed = !arrow.grabbed;

            arrow.position = bow.position; // à modifier car décalage
            arrow.angle = bow.angle - 135;
            arrow.acceleration.x = 0;
            arrow.acceleration.y = gravity;
            arrow.speed = bow.speed;
        }

        if (arrow.grabbed){
            if (IsKeyPressed(KEY_Q)){ // clavier en qwerty ????
                arrow.grabbed = false;

                arrow.position = bow.position; // à modifier car décalage
                arrow.angle = bow.angle - 135;
                arrow.acceleration.x = 0;
                arrow.acceleration.y = gravity;
                arrow.speed.x = fireSpeed * cosf(arrow.angle * PI/180) / screenRatio;
                arrow.speed.y = fireSpeed * sinf(arrow.angle * PI/180) / screenRatio;
            }
            else {
                arrow.acceleration = bow.acceleration;
                arrow.speed = bow.speed;
                arrow.position = bow.position; // à modifier car décalage
                arrow.angle = bow.angle - 135;
            }
        }

        else {          
            
            // gravité flèche
            arrow.speed.y += arrow.acceleration.y * GetFrameTime() * constanteFPS;
        
            // limite vitesse flèche
            if (distance(arrow.speed, (Vector2) {0.0, 0.0}) > arrow.VabsMax) {
                arrow.speed.x = arrow.VabsMax * cosf(arrow.angle * PI / 180);
                arrow.speed.y = arrow.VabsMax * sinf(arrow.angle * PI / 180);
            }
            
            // si collion avec block alors speed = 0 et acceleration = 0

            arrow.position.x += arrow.speed.x * screenRatio * GetFrameTime() * constanteFPS;
            arrow.position.y += arrow.speed.y * screenRatio * GetFrameTime() * constanteFPS;

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
            if ((arrow.position.y + distance((Vector2){arrow.texture.width, arrow.texture.height/2}, (Vector2){0.0, 0.0})) >= currentScreenSize.height/screenRatio - 30){
                // arrow.position.y = currentScreenSize.height/screenRatio - 30;
                arrow.speed.x = 0.0;
                arrow.speed.y = 0.0;
                arrow.acceleration.x = 0.0;
                arrow.acceleration.y = 0.0;
            }
        }
        
// ----------------------------------------------------------------------------------------
//                             Collision handleing
// ----------------------------------------------------------------------------------------

        updateHitboxEntity(&player);
        updateHitboxEntity(&bow);
        updateHitboxEntity(&arrow);

        handleBlockCollisions(&player, map);


// ----------------------------------------------------------------------------------------
//                                   Drawing
// ----------------------------------------------------------------------------------------
        BeginDrawing();
        
        ClearBackground(background_color);
        

        if (IsKeyPressed(KEY_P)){
            camera.zoom += 0.1;
        }
        if (IsKeyPressed(KEY_O)){
            camera.zoom -= 0.1;
        }
        
        limitCameraMap(&camera);

        BeginMode2D(camera);

        cameraFollow(&camera, player);
        limitCameraFollow(&camera, player);

        drawMap(map, blockID);
        /* test affichage map dans le terminal
        if (IsKeyPressed(KEY_T)){
            printf("\nmap =\n");
            for (int j = 0; j<mapSizeY; j++){
                for (int i = 0; i<mapSizeX; i++){
                    printf("%d", map[i*mapSizeY+j]);
                }
                printf("\n");
            }
            printf("\n");
        }
        */
        
        
        
        // affichage des entity dans le bon ordre
        drawEntity(player);
        drawEntity(bow);
        drawEntity(arrow);
        
        /* drawTexturePro bow et arrow
        size = 1;
        DrawTexturePro(bow.texture,
                        (Rectangle) {0, 0, bow.texture.width, bow.texture.height}, // source
                        (Rectangle) {(bow.position.x + bow.texture.width/2)*screenRatio,
                                    (bow.position.y + bow.texture.height/2)*screenRatio,
                                    size*bow.texture.width*screenRatio,
                                    size*bow.texture.height*screenRatio}, // dest
                        (Vector2) {bow.texture.width * size * screenRatio / 2, bow.texture.height * size * screenRatio /2}, // origin
                        bow.angle, WHITE); // angle
        
        // arrow drawing
        DrawTexturePro(arrow.texture,
                        (Rectangle) {0, 0, arrow.texture.width, arrow.texture.height}, // source
                        (Rectangle) {(arrow.position.x + bow.texture.width/2)*screenRatio,
                                    (arrow.position.y + bow.texture.height/2)*screenRatio,
                                    size*arrow.texture.width*screenRatio,
                                    size*arrow.texture.height*screenRatio}, // dest
                        (Vector2) {arrow.texture.width * size * screenRatio / 2, arrow.texture.height * size * screenRatio / 2}, // origin aka le centre de l'image
                                    // dest.width/2, dest.height/2
                        arrow.angle, WHITE); // angle
        */
        
        if (showEntityHitbox){
            drawHitbox(player.hitbox, RED);
            drawHitbox(arrow.hitbox, ORANGE);
            drawHitbox(bow.hitbox, YELLOW);
        }

        // test croix player
        if (IsKeyDown(KEY_C)){
            drawCross((int) player.position.x, (int) player.position.y, BLACK);
        }

        // debug info
        EndMode2D();


// ----------------------------------------------------------------------------------------
//                                   TESTS
// ----------------------------------------------------------------------------------------

        if (showEntityHitbox){
            const char * test0 = TextFormat("entity hitbox ON");
            DrawText(test0, currentScreenSize.width - 180, 10, 20, RED);
        }

        DrawFPS(10, 10);
        const char * test = TextFormat("Frame time = %f", GetFrameTime());
        DrawText(test, 10, 30, 20, BLACK);
        
        
        // test player
        const char * test1 = TextFormat("World position X = %f", player.position.x);
        DrawText(test1, 10, 50, 20, BLACK);
        const char * test2 = TextFormat("World position Y = %f", player.position.y);
        DrawText(test2, 10, 70, 20, BLACK);
        Vector2 testScreen = GetWorldToScreen2D(player.position, camera);
        const char * test01 = TextFormat("Screen position X = %f", testScreen.x);
        DrawText(test01, 10, 90, 20, BLACK);
        const char * test02 = TextFormat("Screen position Y = %f", testScreen.y);
        DrawText(test02, 10, 110, 20, BLACK);
        /*const char * test3 = TextFormat("speed X = %f", player.speed.x);
        DrawText(test3, 10, 50, 20, BLACK);
        const char * test4 = TextFormat("speed Y = %f", player.speed.y);
        DrawText(test4, 10, 70, 20, BLACK);
        */

        /* test arrow
        // test arrow
        const char * test8 = TextFormat("Arrow speed X = %f", arrow.speed.x);
        DrawText(test8, 500, 50, 20, BLACK);
        const char * test9 = TextFormat("Arrow speed Y = %f", arrow.speed.y);
        DrawText(test9, 500, 70, 20, BLACK);
        const char * test10 = TextFormat("Arrow position X = %f", arrow.position.x);
        DrawText(test10, 500, 90, 20, BLACK);
        const char * test11 = TextFormat("Arrow position Y = %f", arrow.position.y);
        DrawText(test11, 500, 110, 20, BLACK);
        const char * test12 = TextFormat("Arrow angle = %d", arrow.angle);
        DrawText(test12, 500, 130, 20, BLACK);
        const char * test15 = TextFormat("Bow speed Y = %f", bow.speed.y);
        DrawText(test15, 500, 150, 20, BLACK);
        */
        
        /* test collisions triangles
        // test collision de 2 triangles
        Vector2 A1 = GetMousePosition();
        Vector2 B1 = {A1.x + 50, A1.y};
        Vector2 C1 = {A1.x + 25, A1.y - 50};
        Vector2 A2 = (Vector2){320, 180};
        Vector2 B2 = (Vector2){370, 180};
        Vector2 C2 = (Vector2){345, 130};
        bool testCollision = checkCollisionTriangles(A1, B1, C1, A2, B2, C2);
        const char * test13 = TextFormat("Collision triangles ? %s", testCollision?"OUI":"NON");
        DrawText(test13, 30, 150, 20, BLACK);
        DrawTriangle(A1, B1, C1, RED);
        DrawTriangle(A2, B2, C2, BLUE);
        */
        
        /* test collision de 2 hitbox
        Hitbox hit1;
        hit1.x = GetMousePosition().x / screenRatio;
        hit1.y = GetMousePosition().y / screenRatio;
        hit1.width = 50;
        hit1.height = 20;
        hit1.angle = 30;
        hit1.A = (Vector2) {400, 200};
        hit1.B = (Vector2) {450, 200};
        hit1.C = (Vector2) {450, 220};
        hit1.D = (Vector2) {400, 220};
        rectToPoints(&hit1);
        drawHitbox(hit1, RED);
        Hitbox hit2;
        hit2.x = 700 / screenRatio;
        hit2.y = 300 / screenRatio;
        hit2.width = 80;
        hit2.height = 15;
        hit2.angle = bow.angle;
        hit2.A = (Vector2) {400, 200};
        hit2.B = (Vector2) {450, 200};
        hit2.C = (Vector2) {450, 220};
        hit2.D = (Vector2) {400, 220};
        rectToPoints(&hit2);
        drawHitbox(hit2, BLUE);
        
        
        bool testCollisionHitbox = checkCollisionHitboxes(hit1, hit2);
        const char * test14 = TextFormat("Collision hitbox ? %s", testCollisionHitbox?"OUI":"NON");
        DrawText(test14, 30, 180, 20, BLACK);
        */
        
        
        EndDrawing();

        frameEnd = GetTime();
        deltaTime = frameEnd - frameStart;
    }
    
    UnloadTexture(air.texture);
    UnloadTexture(dirt.texture);
    UnloadTexture(grass.texture);
    UnloadTexture(stone.texture);
    UnloadTexture(gravel.texture);
    UnloadTexture(player.texture);
    UnloadTexture(bow.texture);
    UnloadTexture(arrow.texture);

    CloseWindow();

    // sauvegarde

    return 0;
}