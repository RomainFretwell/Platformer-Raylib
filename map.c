#include "map.h"

void clearMap(int map[]){
    for (int x = 0; x < mapSize.x; x++){
        for (int y = 0; y < mapSize.y; y++){
            map[x*mapSize.y + y] = 0;
        }
    }
}

void initializeMap(int map[], FILE* fp){
    for (int x = 0; x < mapSize.x; x++){
        for (int y = 0; y < mapSize.y; y++){
            map[x*mapSize.y + y] = 0; // changer pour copier ou upload tableau d'un autre fichier (faire un dossier map)
        }
    }
}

void testMap(int map[]){
    for (int i = 0; i < mapSize.x; i++){
        for (int j = 0; j < mapSize.y; j++){
            map[i*mapSize.y + j] = 0; // changer pour copier ou upload tableau d'un autre fichier (faire un dossier map)
        }
    }

    // map de test en attendant ...
    for (int i = 0; i < mapSize.x; i++){
        map[i*mapSize.y + mapSize.y - 1] = 1;
        map[i*mapSize.y + mapSize.y - 2] = 2;
    }
    for (int i = 20; i < mapSize.x; i++){
        map[i*mapSize.y + mapSize.y - 3] = 4;
    }
    for (int i = 100; i < mapSize.x; i+=3){
        map[i*mapSize.y + mapSize.y - 4] = 3;
    }
    for (int i = 0; i < mapSize.x; i++){
        map[i*mapSize.y] = 1;
        map[i*mapSize.y + 1] = 1;
    }
    for (int j = 0; j < mapSize.y; j++){
        map[j] = 1;
        map[j + mapSize.y] = 1;
        map[j + (mapSize.x-1)*mapSize.y] = 1;
        map[j + (mapSize.x-2)*mapSize.y] = 1;
    }

    for (int i = 20; i < 25; i++){
        map[i*mapSize.y + 20] = 4;
        map[i*mapSize.y + 24] = 4;
    }
    for (int j = 21; j < 24; j++){
        map[20*mapSize.y + j] = 4;
        map[24*mapSize.y + j] = 4;
    }

    for (int i = 33; i < mapSize.x; i++){
        map[i*mapSize.y + 15] = 4;
    }
    for (int j = 15; j < 63; j++){
        map[24*mapSize.y + j] = 4;
        map[32*mapSize.y + j] = 4;
    }

    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 6; j++){
            map[(50+i+8*j)*mapSize.y + mapSize.y-12 - 4*j] = 4;
            map[(50+i+8*j)*mapSize.y + 26 + 4*j] = 4;
        }
    }
    
}



// Right / Left / Up / Down / Corner
typedef enum {
	AIR = -1,
	FULL = 0,
	
	R_WALL = 5,
	L_WALL = 4,
	U_WALL = 1,
	D_WALL = 2,
	
	RU_WALL = 9,
	LU_WALL = 8,
	RD_WALL = 11,
	LD_WALL = 10,
	
	RLU_WALL = 16,
	RL_WALL = 17,
	RLD_WALL = 18,
	
	UDR_WALL = 7,
	UD_WALL = 3,
	UDL_WALL = 6,
	
	RLUD_WALL = 19,
	
	CRU_WALL = 13,
	CLU_WALL = 12,
	CRD_WALL = 15,
	CLD_WALL = 14,
	
} Variant;

/*  Auto Tiling
prend matrice tab de 0=air et 1=block
revoie matrice correspondant avec le bon affichage
*/
void autoTile(bool** tab, int** tiled){
	
	// pour les bords
	for (int i = 0; i<mapSize.x; i++){
		// en haut
		// en bas
	}
	for (int j = 0; j<mapSize.y; j++){
		// à gauche
		// à droite
	}
	
	// pour le reste
	for (int i = 1; i<mapSize.x-1; i++){
		for (int j = 1; j<mapSize.y-1; j++){
			// cas 0 -> air
			if (!tab[i][j]){
				tiled[i][j] = AIR; // -1 pour l'air
			}
			// cas 1 -> blocks partout autour
			else if ( tab[i-1][j-1] && tab[i][j-1] && tab[i+1][j] &&
								tab[i-1][j]   &&                tab[i+1][j] &&
								tab[i-1][j+1] && tab[i][j+1] && tab[i+1][j+1] ){
				tiled[i][j] = FULL ;
			}
			// cas 2
			else if (                 !tab[i][j-1] &&
								tab[i-1][j]   &&                tab[i+1][j] &&
														     tab[i][j+1] ){
				tiled[i][j] = U_WALL;
			}
			// cas 3
			else if (                 !tab[i][j-1] &&
								tab[i-1][j]   &&                tab[i+1][j] &&
														    !tab[i][j+1] ){
				tiled[i][j] = 3;
			}
			// cas 4
			else if (                 !tab[i][j-1] &&
								tab[i-1][j]   &&                tab[i+1][j] &&
								                 tab[i][j+1] ){
				tiled[i][j] = 4;
			}
			// 20 cas ...
		}
	}
}