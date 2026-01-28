#include "map.h"

void clearMap(Map map){
    for (int x = 0; x < map.size.x; x++){
        for (int y = 0; y < map.size.y; y++){
            map.data[x*map.size.y + y] = 0;
			map.tiled[x*map.size.y + y] = 0;
        }
    }
}

void LoadMap(Map map, FILE* fp){
    for (int x = 0; x < map.size.x; x++){
        for (int y = 0; y < map.size.y; y++){
            map.tiled[x*map.size.y + y] = 0; // changer pour copier ou upload tableau d'un autre fichier (faire un dossier map)
        }
    }
}

void testMap(Map map){
    // map de test en attendant ...
    for (int i = 0; i < map.size.x; i++){
        map.data[i*map.size.y + map.size.y - 1] = 1;
		map.data[i*map.size.y + map.size.y - 2] = 1;
    }
    for (int i = 20; i < map.size.x; i++){
        map.data[i*map.size.y + map.size.y - 3] = 1;
    }
    for (int i = 100; i < map.size.x; i+=3){
        map.data[i*map.size.y + map.size.y - 4] = 1;
    }
    for (int i = 0; i < map.size.x; i++){
        map.data[i*map.size.y] = 1;
        map.data[i*map.size.y + 1] = 1;
    }
    for (int j = 0; j < map.size.y; j++){
        map.data[j] = 1;
        map.data[j + map.size.y] = 1;
        map.data[j + (map.size.x-1)*map.size.y] = 1;
        map.data[j + (map.size.x-2)*map.size.y] = 1;
    }

    for (int i = 20; i < 25; i++){
        map.data[i*map.size.y + 20] = 1;
        map.data[i*map.size.y + 24] = 1;
    }
    for (int j = 21; j < 24; j++){
        map.data[20*map.size.y + j] = 1;
        map.data[24*map.size.y + j] = 1;
    }

    for (int i = 33; i < map.size.x; i++){
        map.data[i*map.size.y + 15] = 1;
    }
    for (int j = 15; j < 63; j++){
        map.data[24*map.size.y + j] = 1;
        map.data[32*map.size.y + j] = 1;
    }

    for (int i = 80; i < map.size.x; i+=7){
        map.data[i*map.size.y + 30] = 1;
    }
    
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 6; j++){
            map.data[(50+i+8*j)*map.size.y + map.size.y-12 - 4*j] = 1;
            map.data[(50+i+8*j)*map.size.y + 26 + 4*j] = 1;
        }
    }
    
}



/*  Auto Tiling
prend matrice tab de 0=air et 1=block
revoie matrice correspondant avec le bon affichage
*/

void autoTile(Map map){

	// pour les bords
	for (int i = 0; i<map.size.x; i++){
		map.tiled[i*map.size.y] = FULL_WALL;
		map.tiled[i*map.size.y + map.size.y-1] = FULL_WALL;
	}
	for (int j = 0; j<map.size.y; j++){
		map.tiled[j] = FULL_WALL;
		map.tiled[map.size.x-1 + j] = FULL_WALL;
	}
	
	// pour le reste
	for (int i = 1; i<map.size.x-1; i++){
		for (int j = 1; j<map.size.y-1; j++){
			// air
            if (!map.data[i*map.size.y + j]){
				map.tiled[i*map.size.y + j] = -1; // -1 pour l'air
			}
            // blocks partout autour
			else if (	map.data[(i-1)*map.size.y + j-1] && map.data[i*map.size.y + j-1] && map.data[(i+1)*map.size.y + j-1] &&
						map.data[(i-1)*map.size.y + j]   && 							    map.data[(i+1)*map.size.y + j]   &&
						map.data[(i-1)*map.size.y + j+1] && map.data[i*map.size.y + j+1] && map.data[(i+1)*map.size.y + j+1] ){
				map.tiled[i*map.size.y + j] = FULL_WALL ;
			}
            // U_WALL
			else if (                                       !map.data[i*map.size.y + j-1] && 
						map.data[(i-1)*map.size.y + j]  && 							        map.data[(i+1)*map.size.y + j]   &&
						                                    map.data[i*map.size.y + j+1]  ){
				map.tiled[i*map.size.y + j] = U_WALL;
			}
			// D_WALL
			else if (	                                map.data[i*map.size.y + j-1] && 
						map.data[(i-1)*map.size.y + j] && 							    map.data[(i+1)*map.size.y + j] &&
						                                !map.data[i*map.size.y + j+1] ){
				map.tiled[i*map.size.y + j] = D_WALL;
            }
			// UD_WALL
            else if (	                                !map.data[i*map.size.y + j-1] && 
						map.data[(i-1)*map.size.y + j] && 							    map.data[(i+1)*map.size.y + j] &&
						                                !map.data[i*map.size.y + j+1] ){
				map.tiled[i*map.size.y + j] = UD_WALL;
            }
            // L_WALL
            else if (	                                map.data[i*map.size.y + j-1] && 
						!map.data[(i-1)*map.size.y + j] && 							    map.data[(i+1)*map.size.y + j] &&
						                                map.data[i*map.size.y + j+1] ){
				map.tiled[i*map.size.y + j] = L_WALL;
            }
            // R_WALL
            else if (	                                map.data[i*map.size.y + j-1] && 
						map.data[(i-1)*map.size.y + j] && 							    !map.data[(i+1)*map.size.y + j] &&
						                                map.data[i*map.size.y + j+1] ){
				map.tiled[i*map.size.y + j] = R_WALL;
            }
            // UDL_WALL
            else if (	                                !map.data[i*map.size.y + j-1] && 
						!map.data[(i-1)*map.size.y + j] && 							    map.data[(i+1)*map.size.y + j] &&
						                                !map.data[i*map.size.y + j+1] ){
				map.tiled[i*map.size.y + j] = UDL_WALL;
            }
            // UDR_WALL
            else if (	                                !map.data[i*map.size.y + j-1] && 
						map.data[(i-1)*map.size.y + j] && 							    !map.data[(i+1)*map.size.y + j] &&
						                                !map.data[i*map.size.y + j+1] ){
				map.tiled[i*map.size.y + j] = UDR_WALL;
            }
            // LU_WALL
            else if (	                                !map.data[i*map.size.y + j-1] && 
						!map.data[(i-1)*map.size.y + j] && 							    map.data[(i+1)*map.size.y + j] &&
						                                map.data[i*map.size.y + j+1] ){
				map.tiled[i*map.size.y + j] = LU_WALL;
            }
            // RU_WALL
            else if (	                                !map.data[i*map.size.y + j-1] && 
						map.data[(i-1)*map.size.y + j] && 							    !map.data[(i+1)*map.size.y + j] &&
						                                map.data[i*map.size.y + j+1] ){
				map.tiled[i*map.size.y + j] = RU_WALL;
            }
            // LD_WALL
            else if (	                                map.data[i*map.size.y + j-1] && 
						!map.data[(i-1)*map.size.y + j] && 							    map.data[(i+1)*map.size.y + j] &&
						                                !map.data[i*map.size.y + j+1] ){
				map.tiled[i*map.size.y + j] = LD_WALL;
            }
            // RD_WALL
            else if (	                                map.data[i*map.size.y + j-1] && 
						map.data[(i-1)*map.size.y + j] && 							    !map.data[(i+1)*map.size.y + j] &&
						                                !map.data[i*map.size.y + j+1] ){
				map.tiled[i*map.size.y + j] = RD_WALL;
            }
            // CLU_WALL
            else if (	!map.data[(i-1)*map.size.y + j-1] && map.data[i*map.size.y + j-1] && 
						map.data[(i-1)*map.size.y + j]   && 							    map.data[(i+1)*map.size.y + j] &&
						                                    map.data[i*map.size.y + j+1]  ){
				map.tiled[i*map.size.y + j] = CLU_WALL ;
			}
            // CRU_WALL
            else if (	                                    map.data[i*map.size.y + j-1] && !map.data[(i+1)*map.size.y + j-1] &&
						map.data[(i-1)*map.size.y + j]   && 							    map.data[(i+1)*map.size.y + j]   &&
						                                    map.data[i*map.size.y + j+1] ){
				map.tiled[i*map.size.y + j] = CRU_WALL ;
			}
            // CLD_WALL
            else if (	                                    map.data[i*map.size.y + j-1] &&
						map.data[(i-1)*map.size.y + j]   && 							    map.data[(i+1)*map.size.y + j]   &&
						!map.data[(i-1)*map.size.y + j+1] && map.data[i*map.size.y + j+1] ){
				map.tiled[i*map.size.y + j] = CLD_WALL ;
			}
            // CRD_WALL
            else if (	                                    map.data[i*map.size.y + j-1] && 
						map.data[(i-1)*map.size.y + j]   && 							    map.data[(i+1)*map.size.y + j]   &&
						                                    map.data[i*map.size.y + j+1] && !map.data[(i+1)*map.size.y + j+1] ){
				map.tiled[i*map.size.y + j] = CRD_WALL ;
			}
            // RLU_WALL
            else if (	                                !map.data[i*map.size.y + j-1] && 
						!map.data[(i-1)*map.size.y + j] && 							    !map.data[(i+1)*map.size.y + j] &&
						                                map.data[i*map.size.y + j+1] ){
				map.tiled[i*map.size.y + j] = RLU_WALL;
            }
            // RL_WALL
            else if (	                                map.data[i*map.size.y + j-1] && 
						!map.data[(i-1)*map.size.y + j] && 							    !map.data[(i+1)*map.size.y + j] &&
						                                map.data[i*map.size.y + j+1] ){
				map.tiled[i*map.size.y + j] = RL_WALL;
            }
            // RLD_WALL
            else if (	                                map.data[i*map.size.y + j-1] && 
						!map.data[(i-1)*map.size.y + j] && 							    !map.data[(i+1)*map.size.y + j] &&
						                                !map.data[i*map.size.y + j+1] ){
				map.tiled[i*map.size.y + j] = RLD_WALL;
            }
            // RLUD_WALL
            else if (	                                !map.data[i*map.size.y + j-1] && 
						!map.data[(i-1)*map.size.y + j] && 							    !map.data[(i+1)*map.size.y + j] &&
						                                !map.data[i*map.size.y + j+1] ){
				map.tiled[i*map.size.y + j] = RLUD_WALL;
            }
            else printf("erreur dans autoTile\n");
		}
	}
}
