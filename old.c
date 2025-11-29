#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"


// ----------------------------------------------------------------------------------------
//                                   Constants
// ----------------------------------------------------------------------------------------
#define nbBlock 5
#define blockSize 15 // les blocks sont de taille 15x15
#define sizeCoef 1 // multiplier blockSize par sizeCoef pour des blocks plus gros

// ----------------------------------------------------------------------------------------
//                                   Structures
// ----------------------------------------------------------------------------------------

typedef struct structVector2int {
    int width;
    int height;
} Vector2int;

typedef struct structHitbox {
    float x;
    float y;
    float width;
    float height;
    float angle;
    // autre representation d'un rectangle ABCD avec 4 points
    Vector2 A;
    Vector2 B;
    Vector2 C;
    Vector2 D;
} Hitbox; // rectangle with rotation

typedef struct structEntity {
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    Vector2 Vmax;
    float angle;
    int direction;
    Texture2D texture;
    Hitbox hitbox;
    bool showHitbox;
} Entity;

typedef struct structBlock {
    Vector2 position;
    Texture2D texture;
    Hitbox hitbox;
    bool showHitbox;
    float coefRebond;
} Block;

typedef struct structItem {
    bool grabbed;
    Vector2 position; // "same" as player if grabbed
    Vector2 velocity; // if !grabbed
    Vector2 acceleration; // if !grabbed
    float Vmax;
    float angle;
    Texture2D texture;
    Hitbox hitbox;
    bool showHitbox;
} Item;


// ----------------------------------------------------------------------------------------
//                                   Global variables
// ----------------------------------------------------------------------------------------
Vector2int fullScreenSize;
Vector2int smallScreenSize = {960, 540};
Vector2int currentScreenSize;
float screenRatio;


// ----------------------------------------------------------------------------------------
//                                   Math functions
// ----------------------------------------------------------------------------------------

float distance(Vector2 objetA, Vector2 objetB){
    return sqrtf((objetA.x - objetB.x) * (objetA.x - objetB.x) + (objetA.y - objetB.y) * (objetA.y - objetB.y));
}

int signe(float x){
    if (x > 0) return 1;
    else if (x < 0) return -1;
    else return 0;
}

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

void initializeMap(int map[], int width, int height){
    for (int x = 0; x < width; x++){
        for (int y = 0; y < height; y++){
            map[x*height + y] = y%2 + 2;
        }
    }
}

void drawMap(int map[], int mapSizeX, int mapSizeY, Block blockID[]){
    int i;
    for (int x = 0; x < mapSizeX; x++){
        for (int y = 0; y < mapSizeY; y++){
            i = map[x*mapSizeY + y];
            DrawTextureEx(blockID[i].texture, (Vector2){x*blockSize*sizeCoef*screenRatio, y*blockSize*sizeCoef*screenRatio}, 0, sizeCoef*screenRatio, WHITE);
        }
    }
}

void drawHitbox(Hitbox hitbox, Color color){
    // AB
    DrawLine(screenRatio*hitbox.A.x, screenRatio*hitbox.A.y, screenRatio*hitbox.B.x, screenRatio*hitbox.B.y, color);
    // BC
    DrawLine(screenRatio*hitbox.B.x, screenRatio*hitbox.B.y, screenRatio*hitbox.C.x, screenRatio*hitbox.C.y, color);
    // CD
    DrawLine(screenRatio*hitbox.C.x, screenRatio*hitbox.C.y, screenRatio*hitbox.D.x, screenRatio*hitbox.D.y, color);
    // AD
    DrawLine(screenRatio*hitbox.A.x, screenRatio*hitbox.A.y, screenRatio*hitbox.D.x, screenRatio*hitbox.D.y, color);
}

void drawEntity(Entity ent){
    int size = 1;
    DrawTexturePro( ent.texture,
                    (Rectangle) {0, 0, ent.direction * ent.texture.width, ent.texture.height}, // source
                    (Rectangle) {(ent.position.x + ent.texture.width/2)*screenRatio,
                                (ent.position.y + ent.texture.height/2)*screenRatio,
                                size*ent.texture.width*screenRatio,
                                size*ent.texture.height*screenRatio}, // dest
                    (Vector2) {ent.texture.width/2 * screenRatio, ent.texture.height/2 * screenRatio}, // origin
                    ent.angle, 
                    WHITE);
}

void drawItem(Item item){
    int size = 1;
    DrawTexturePro( item.texture,
                    (Rectangle) {0, 0, item.texture.width, item.texture.height}, // source
                    (Rectangle) {(item.position.x + item.texture.width/2)*screenRatio,
                                (item.position.y + item.texture.height/2)*screenRatio,
                                size*item.texture.width*screenRatio,
                                size*item.texture.height*screenRatio}, // dest
                    (Vector2) {item.texture.width/2 * screenRatio, item.texture.height/2 * screenRatio}, // origin
                    item.angle, 
                    WHITE);
}

bool checkCollisionTriangles(Vector2 A1, Vector2 B1, Vector2 C1, Vector2 A2, Vector2 B2, Vector2 C2){
    if (CheckCollisionPointTriangle(A1, A2, B2, C2)) return true;
    if (CheckCollisionPointTriangle(B1, A2, B2, C2)) return true;
    if (CheckCollisionPointTriangle(C1, A2, B2, C2)) return true;
    if (CheckCollisionPointTriangle(A2, A1, B1, C1)) return true;
    if (CheckCollisionPointTriangle(B2, A1, B1, C1)) return true;
    if (CheckCollisionPointTriangle(C2, A1, B1, C1)) return true;
    return false;
}

// utile ? pê...
void pointsToRect(Hitbox hitbox){
    // TODO
}

void rectToPoints(Hitbox *hitbox){
    hitbox->A = (Vector2) {hitbox->x, hitbox->y};
    hitbox->B = (Vector2) {hitbox->A.x + hitbox->width*cosf(hitbox->angle * PI/180), hitbox->A.y + hitbox->width*sinf(hitbox->angle * PI/180)};
    hitbox->D = (Vector2) {hitbox->A.x - hitbox->height*sinf(hitbox->angle * PI/180), hitbox->A.y + hitbox->height*cosf(hitbox->angle * PI/180)};
    hitbox->C = (Vector2) {hitbox->B.x + hitbox->D.x - hitbox->A.x, hitbox->B.y + hitbox->D.y - hitbox->A.y};
}

bool checkCollisionHitboxes(Hitbox hitbox1, Hitbox hitbox2){

    rectToPoints(&hitbox1);
    rectToPoints(&hitbox2);

    if (checkCollisionTriangles(hitbox1.A, hitbox1.B, hitbox1.C, hitbox2.A , hitbox2.B , hitbox2.C)) return 1;
    if (checkCollisionTriangles(hitbox1.A, hitbox1.B, hitbox1.C, hitbox2.A , hitbox2.D , hitbox2.C)) return 1;
    if (checkCollisionTriangles(hitbox1.A, hitbox1.D, hitbox1.C, hitbox2.A , hitbox2.B , hitbox2.C)) return 1;
    if (checkCollisionTriangles(hitbox1.A, hitbox1.D, hitbox1.C, hitbox2.A , hitbox2.D , hitbox2.C)) return 1;

    drawHitbox(hitbox1, RED);
    drawHitbox(hitbox2, BLUE);

    return false;
}

void updateHitboxEntity(Entity *ent){ // ajouter décalage en parametre
    ent->hitbox.x = ent->position.x; // à modifier ?
    ent->hitbox.y = ent->position.y; // à modifier ?
    ent->hitbox.width = ent->texture.width; // à enlever ?
    ent->hitbox.height = ent->texture.height; // à enlever ?
    ent->hitbox.angle = ent->angle;
    rectToPoints(&(ent->hitbox));
}

void updateHitboxItem(Item *item){ // ajouter décalage en parametre
    item->hitbox.x = item->position.x; // à modifier ?
    item->hitbox.y = item->position.y; // à modifier ?
    item->hitbox.width = item->texture.width; // à enlever ?
    item->hitbox.height = item->texture.height; // à enlever ?
    item->hitbox.angle = item->angle;
    rectToPoints(&(item->hitbox));
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
    
    Block air;
    Block dirt;
    Block grass;
    Block stone;
    Block gravel;
    
    Block blockID[nbBlock] = {air, dirt, grass, stone, gravel};

    // Load block textures
    air.texture = LoadTexture("resources/dirt.png"); // transparent texture
    dirt.texture = LoadTexture("resources/dirt.png");
    grass.texture = LoadTexture("resources/grass.png");
    stone.texture = LoadTexture("resources/stone.png");
    gravel.texture = LoadTexture("resources/gravel.png");
    
    // Map initialization
    // Texture2D tableauTextureBlock[nbBlock] = {air, dirt, grass, stone, gravel};
    int mapSizeX = 64;
    int mapSizeY = 36;
    int map[mapSizeX * mapSizeY];
    initializeMap(map, mapSizeX, mapSizeY);
    map[100] = 4;

    Color background_color = {220, 230, 255, 255};

    float gravity = 0.3;

    // player initialization
    Entity player;
    player.position = (Vector2){0, 0};
    player.Vmax = (Vector2){5.0, 10.0};
    player.velocity = (Vector2){0, 0};
    player.acceleration = (Vector2){0.0, gravity};
    float acceleration_ground = 0.1;
    player.angle = 0;
    player.direction = 1;
    player.texture = LoadTexture("resources/druid.png");
    updateHitboxEntity(&player);
    
    // bow
    Item bow;
    bow.position = (Vector2){currentScreenSize.width/2, currentScreenSize.height/2};
    bow.velocity = player.velocity;
    bow.acceleration = player.acceleration;
    bow.angle = 135;
    bow.texture = LoadTexture("resources/bow.png");

    Item arrow;
    arrow.position =  bow.position; // à changer à player.position
    arrow.velocity = player.velocity;
    arrow.acceleration = player.acceleration; // pas de gravité si pas tirée
    int fireSpeed = 10;
    arrow.Vmax = 50.0;
    arrow.angle = 0;
    arrow.grabbed = true;
    arrow.texture = LoadTexture("resources/arrow.png");
    updateHitboxItem(&arrow);
    
    // other variables
    float jumpCooldown = 1;
    float coefRebond = 0.1; // 0 = pas de rebond, 1 = rebond parfait
    float testAngleArc = 1;

    // time variables
    double frameStart;
    double frameEnd;
    double deltaTime = 0.01;
    int size = 1;

    float constanteFPS = 80.0;
    int maxFPS = 200;
    SetTargetFPS(maxFPS);


// ----------------------------------------------------------------------------------------
//                                   Game loop
// ----------------------------------------------------------------------------------------

    while (!WindowShouldClose()){
        
        frameStart = GetTime();
        
        // Toggle fullscreen
        if (IsKeyPressed(KEY_F11)) ToggleFullscreenWindow();
                

        if (IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT) && player.velocity.x < player.Vmax.x){
            player.velocity.x += acceleration_ground;
            player.direction = 1;
        }
        else if (IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT) && player.velocity.x > -player.Vmax.x){
            player.velocity.x -= acceleration_ground;
            player.direction = -1;
        }
        // ralentissement friction sol
        else if (!IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)){
            if (player.velocity.x > 0){
                player.velocity.x -= acceleration_ground;
                if (player.velocity.x < 0) player.velocity.x = 0.0;
            }
            if (player.velocity.x < 0){
                player.velocity.x += acceleration_ground;
                if (player.velocity.x > 0) player.velocity.x = 0.0;
            }
            if (-0.1 < player.velocity.x && player.velocity.x < 0.1){
                player.velocity.x = 0.0;
                player.acceleration.x = 0.0;
            }
        }

        if (IsKeyDown(KEY_SPACE)){
            player.position.y = 0;
            player.velocity.y = 0;
            player.acceleration.y = gravity;
        }
        
        // player jump
        if (IsKeyDown(KEY_UP) && canJump(player)){
            player.velocity.y = - 2 * player.Vmax.y;
            player.acceleration.y = gravity;
            // jumpCooldown = 1;
        }

        // gravité player
        player.velocity.y += player.acceleration.y * GetFrameTime() * constanteFPS;
        // limite vitesse player
        if (player.velocity.x > player.Vmax.x) player.velocity.x = player.Vmax.x;
        if (player.velocity.y > player.Vmax.y) player.velocity.y = player.Vmax.y;
        if (player.velocity.x < -player.Vmax.x) player.velocity.x = -player.Vmax.x;
        if (player.velocity.y < -player.Vmax.y) player.velocity.y = -player.Vmax.y;

        // Rebond
        if (player.position.y * screenRatio >= currentScreenSize.height-(2*blockSize*sizeCoef + player.texture.height)*screenRatio){
            player.position.y = currentScreenSize.height / screenRatio - (2*blockSize*sizeCoef + player.texture.height);
            if (player.velocity.y <= 2.){
                player.velocity.y = 0;
                player.acceleration.y = 0;
            }
            else if (jumpCooldown == 1){
                player.velocity.y *= -coefRebond;
                player.acceleration.y = gravity;
            }
        }
        jumpCooldown -= 0.1 * GetFrameTime() * 200;

        player.position.x += player.velocity.x * screenRatio * GetFrameTime() * constanteFPS;
        player.position.y += player.velocity.y * screenRatio * GetFrameTime() * constanteFPS;

        float totalSpeed = distance(player.velocity, (Vector2){0, 0});
        
        // player angle
        if (player.velocity.x == 0){
            if (player.velocity.y < 0){
                player.angle = 90;
            }
            else if (player.velocity.y > 0){
                player.angle = -90;
            }
            else {
                player.angle = 0;
            }
        }
        else {
            player.angle = (int) (atan(player.velocity.y / player.velocity.x) * 180/PI);
        }

        // limites angles
        if (player.angle > 60) player.angle = 60;
        if (player.angle < -60) player.angle = -60;
        
        if (IsKeyPressed(KEY_H)){
            player.showHitbox = !player.showHitbox;
        }

// ----------------------------------------------------------------------------------------
//                                   Arrow
// ----------------------------------------------------------------------------------------

        // test arc et flèche qui tourne
        /*if (bow.angle >= 135+90 || bow.angle <= 45-180){
            testAngleArc *= -1;
        }*/
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
            arrow.velocity = bow.velocity;
        }

        if (arrow.grabbed){
            if (IsKeyPressed(KEY_Q)){ // clavier en qwerty ????
                arrow.grabbed = false;

                arrow.position = bow.position; // à modifier car décalage
                arrow.angle = bow.angle - 135;
                arrow.acceleration.x = 0;
                arrow.acceleration.y = gravity;
                arrow.velocity.x = fireSpeed * cosf(arrow.angle * PI/180) / screenRatio;
                arrow.velocity.y = fireSpeed * sinf(arrow.angle * PI/180) / screenRatio;
            }
            else {
                arrow.acceleration = bow.acceleration;
                arrow.velocity = bow.velocity;
                arrow.position = bow.position; // à modifier car décalage
                arrow.angle = bow.angle - 135;
            }
        }

        else {          
            
            // gravité flèche
            arrow.velocity.y += arrow.acceleration.y * GetFrameTime() * constanteFPS;
        
            // limite vitesse flèche
            if (distance(arrow.velocity, (Vector2) {0.0, 0.0}) > arrow.Vmax) {
                arrow.velocity.x = arrow.Vmax * cosf(arrow.angle * PI / 180);
                arrow.velocity.y = arrow.Vmax * sinf(arrow.angle * PI / 180);
            }
            
            // si collion avec block alors velocity = 0 et acceleration = 0

            arrow.position.x += arrow.velocity.x * screenRatio * GetFrameTime() * constanteFPS;
            arrow.position.y += arrow.velocity.y * screenRatio * GetFrameTime() * constanteFPS;

            // arrow angle
            if (arrow.velocity.x != 0 || arrow.velocity.y != 0){
                if (arrow.velocity.y == 0){
                    arrow.angle = 180 * (1-signe(arrow.velocity.x))/2;
                }
                if (arrow.velocity.x == 0){
                    arrow.angle = - 90 * signe(arrow.velocity.y);
                }
                else {
                    if (arrow.velocity.x > 0){
                        arrow.angle = (int) (atan(arrow.velocity.y / arrow.velocity.x) * 180/PI);
                    }
                    else {
                        arrow.angle = 180 + (int) (atan(arrow.velocity.y / arrow.velocity.x) * 180/PI);
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
                arrow.velocity.x = 0.0;
                arrow.velocity.y = 0.0;
                arrow.acceleration.x = 0.0;
                arrow.acceleration.y = 0.0;
            }
        }
        

// ----------------------------------------------------------------------------------------
//                             Collision handleing
// ----------------------------------------------------------------------------------------

        //if (checkCollisionHitboxes(player.hitbox, gravel.hitbox))


// ----------------------------------------------------------------------------------------
//                                   Drawing
// ----------------------------------------------------------------------------------------

        BeginDrawing();

            ClearBackground(background_color);
            
            for (int x = 0; x < currentScreenSize.width; x++){
                DrawTextureEx(dirt.texture, (Vector2){x*blockSize*sizeCoef*screenRatio, currentScreenSize.height-blockSize*sizeCoef*screenRatio}, 0, screenRatio, WHITE);
                DrawTextureEx(grass.texture, (Vector2){x*blockSize*sizeCoef*screenRatio, currentScreenSize.height-2*blockSize*sizeCoef*screenRatio}, 0, screenRatio, WHITE);
            }
            DrawTextureEx(gravel.texture, (Vector2){10*blockSize*sizeCoef*screenRatio, currentScreenSize.height-3*blockSize*sizeCoef*screenRatio}, 0, screenRatio, WHITE);
            
            // entities
            drawEntity(player);
            drawItem(bow);
            drawItem(arrow);
            
            /*
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

            updateHitboxEntity(&player);
            if (player.showHitbox){
                drawHitbox(player.hitbox, ORANGE);
                const char * test0 = TextFormat("hitbox ON");
                DrawText(test0, 10, 100, 20, ORANGE);
            }
            updateHitboxItem(&arrow);
            if (arrow.showHitbox){
                drawHitbox(arrow.hitbox, RED);
                const char * test20 = TextFormat("hitbox ON");
                DrawText(test20, 10, 120, 20, RED);
            }
            updateHitboxItem(&bow);
            if (bow.showHitbox){
                drawHitbox(bow.hitbox, YELLOW);
                const char * test21 = TextFormat("hitbox ON");
                DrawText(test21, 10, 140, 20, YELLOW);
            }
            
            
            // DrawTextureEx(dirt, (Vector2){currentScreenSize.x/2, currentScreenSize.y/2}, angle, 4*screenRatio, WHITE);
            // drawMap(map, mapSizeX, mapSizeY, tableauTextureBlock, currentScreenSize.x, currentScreenSize.y, screenRatio, screenRatio);
            

            // debug info
            DrawFPS(10, 10);
            const char * test = TextFormat("Frame time = %f", GetFrameTime());
            DrawText(test, 10, 30, 20, BLACK);

            /*

            // test player
            const char * test2 = TextFormat("Velocity X = %f", player.velocity.x);
            DrawText(test2, 10, 50, 20, BLACK);
            */
            const char * test3 = TextFormat("Velocity Y = %f", player.velocity.y);
            DrawText(test3, 10, 70, 20, BLACK);
            /*
            const char * test4 = TextFormat("Position X = %f", player.position.x);
            DrawText(test4, 10, 90, 20, BLACK);
            const char * test5 = TextFormat("Position Y = %f", player.position.y);
            DrawText(test5, 10, 110, 20, BLACK);
            const char * test6 = TextFormat("Angle = %d", player.angle);
            DrawText(test6, 10, 130, 20, BLACK);
            const char * test7 = TextFormat("direction = %s", ((1 + player.direction)/2)?"1 (droite)":"-1 (gauche)");
            DrawText(test7, 10, 150, 20, BLACK);
            
            */

            // test arrow
            const char * test8 = TextFormat("Arrow velocity X = %f", arrow.velocity.x);
            DrawText(test8, 500, 50, 20, BLACK);
            const char * test9 = TextFormat("Arrow velocity Y = %f", arrow.velocity.y);
            DrawText(test9, 500, 70, 20, BLACK);
            const char * test10 = TextFormat("Arrow position X = %f", arrow.position.x);
            DrawText(test10, 500, 90, 20, BLACK);
            const char * test11 = TextFormat("Arrow position Y = %f", arrow.position.y);
            DrawText(test11, 500, 110, 20, BLACK);
            const char * test12 = TextFormat("Arrow angle = %d", arrow.angle);
            DrawText(test12, 500, 130, 20, BLACK);

            const char * test15 = TextFormat("Bow velocity Y = %f", bow.velocity.y);
            DrawText(test15, 500, 150, 20, BLACK);

            /*
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

            Hitbox hit;
            hit.x = 600 * screenRatio;
            hit.y = 200 * screenRatio;
            hit.width = 50 * screenRatio;
            hit.height = 20 * screenRatio;
            hit.angle = bow.angle;
            hit.A = (Vector2) {400, 200};
            hit.B = (Vector2) {450, 200};
            hit.C = (Vector2) {450, 220};
            hit.D = (Vector2) {400, 220};
            
            rectToPoints(&hit);
            
            drawHitbox(hit, RED);

            // croix
            DrawLine(0, hit.A.y, currentScreenSize.width, hit.A.y, BLACK);
            DrawLine(hit.A.x, 0, hit.A.x, currentScreenSize.height, BLACK);

            DrawLine(0, hit.B.y, currentScreenSize.width, hit.B.y, GREEN);
            DrawLine(hit.B.x, 0, hit.B.x, currentScreenSize.height, GREEN);
            
            // test collision de 2 hitbox
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
            drawHitbox(hit2, BLUE);
            
            
            bool testCollisionHitbox = checkCollisionHitboxes(hit1, hit2);
            const char * test14 = TextFormat("Collision hitbox ? %s", testCollisionHitbox?"OUI":"NON");
            DrawText(test14, 30, 180, 20, BLACK);
            
            /* 
            
            Problème = marche pas si forme de pensement en croix
            Pas vraiment un problème vu que il faut d'abord traverser l'objet (et donc avoir une collision) pour se mettre dans cette position
            peut être un problème si qlqch va trop vite ?
            
            CheckCollisionLines ?

            Vrai solution = algorythme SAT -> sur youtube 

            Ma version des collisions devrait suffire même si elle est fausse
            -> elle semble plus rapide aussi
            -> si jamais j'ai un porblème de colision alors on pourra coder SAT
            
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