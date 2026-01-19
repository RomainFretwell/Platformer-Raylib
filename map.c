#include "map.h"

void clearMap(int map[]){
    for (int x = 0; x < mapSizeX; x++){
        for (int y = 0; y < mapSizeY; y++){
            map[x*mapSizeY + y] = 0;
        }
    }
}

void initializeMap(int map[], FILE* fp){
    for (int x = 0; x < mapSizeX; x++){
        for (int y = 0; y < mapSizeY; y++){
            map[x*mapSizeY + y] = 0; // changer pour copier ou upload tableau d'un autre fichier (faire un dossier map)
        }
    }
}

void testMap(int map[]){
    for (int x = 0; x < mapSizeX; x++){
        for (int y = 0; y < mapSizeY; y++){
            map[x*mapSizeY + y] = 0; // changer pour copier ou upload tableau d'un autre fichier (faire un dossier map)
        }
    }

    // map de test en attendant ...
    for (int x = 0; x < mapSizeX; x++){
        map[x*mapSizeY + mapSizeY - 1] = 1;
        map[x*mapSizeY + mapSizeY - 2] = 2;
    }
    for (int x = 20; x < mapSizeX; x++){
        map[x*mapSizeY + mapSizeY - 3] = 4;
    }
    for (int x = 100; x < mapSizeX; x+=3){
        map[x*mapSizeY + mapSizeY - 4] = 3;
    }
    for (int x = 0; x < mapSizeX; x++){
        map[x*mapSizeY] = 1;
        map[x*mapSizeY + 1] = 1;
    }
    for (int y = 0; y < mapSizeY; y++){
        map[y] = 1;
        map[y + mapSizeY] = 1;
        map[y + (mapSizeX-1)*mapSizeY] = 1;
        map[y + (mapSizeX-2)*mapSizeY] = 1;
    }

    for (int x = 20; x < 25; x++){
        map[x*mapSizeY + 20] = 4;
        map[x*mapSizeY + 24] = 4;
    }
    for (int y = 21; y < 24; y++){
        map[20*mapSizeY + y] = 4;
        map[24*mapSizeY + y] = 4;
    }

    for (int x = 33; x < mapSizeX; x++){
        map[x*mapSizeY + 15] = 4;
    }
    for (int y = 15; y < 64; y++){
        map[24*mapSizeY + y] = 4;
        map[32*mapSizeY + y] = 4;
    }

    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 6; j++){
            map[(50+i+8*j)*mapSizeY + mapSizeY-12 - 4*j] = 4;
            map[(50+i+8*j)*mapSizeY + 26 + 4*j] = 4;
        }
    }
    
}