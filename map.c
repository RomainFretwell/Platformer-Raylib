#include "map.h"

void clearMap(Map map){
    for (int x = 0; x < map.size.x; x++){
        for (int y = 0; y < map.size.y; y++){
            map.data[x*map.size.y + y] = 0;
			map.tiled[x*map.size.y + y] = 0;
        }
    }
}

void LoadMap(Map* map, FILE* fp){
    for (int x = 0; x < map->size.x; x++){
        for (int y = 0; y < map->size.y; y++){
            map->tiled[x*map->size.y + y] = 0; // changer pour copier ou upload tableau d'un autre fichier (faire un dossier map)
        }
    }
}

void testMap(Map* map){
    // map de test en attendant ...
    for (int i = 0; i < map->size.x; i++){
        map->data[i*map->size.y + map->size.y - 1] = 1;
		map->data[i*map->size.y + map->size.y - 2] = 1;
    }
    for (int i = 20; i < map->size.x; i++){
        map->data[i*map->size.y + map->size.y - 3] = 1;
    }
    for (int i = 100; i < map->size.x; i+=3){
        map->data[i*map->size.y + map->size.y - 4] = 1;
    }
    for (int i = 0; i < map->size.x; i++){
        map->data[i*map->size.y] = 1;
        map->data[i*map->size.y + 1] = 1;
    }
    for (int j = 0; j < map->size.y; j++){
        map->data[j] = 1;
        map->data[j + map->size.y] = 1;
        map->data[j + (map->size.x-1)*map->size.y] = 1;
        map->data[j + (map->size.x-2)*map->size.y] = 1;
    }

    for (int i = 20; i < 25; i++){
        map->data[i*map->size.y + 20] = 1;
        map->data[i*map->size.y + 24] = 1;
    }
    for (int j = 21; j < 24; j++){
        map->data[20*map->size.y + j] = 1;
        map->data[24*map->size.y + j] = 1;
    }

    for (int i = 33; i < map->size.x; i++){
        map->data[i*map->size.y + 15] = 1;
    }
    for (int j = 15; j < 63; j++){
        map->data[24*map->size.y + j] = 1;
        map->data[32*map->size.y + j] = 1;
    }

    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 6; j++){
            map->data[(50+i+8*j)*map->size.y + map->size.y-12 - 4*j] = 1;
            map->data[(50+i+8*j)*map->size.y + 26 + 4*j] = 1;
        }
    }
    
}



/*  Auto Tiling
prend matrice tab de 0=air et 1=block
revoie matrice correspondant avec le bon affichage
*/

void autoTile(Map map){
	/*


	// pour les bords
	for (int i = 0; i<map.size.x; i++){
		// en haut
		// en bas
	}
	for (int j = 0; j<map.size.y; j++){
		// à gauche
		// à droite
	}
	
	// pour le reste
	for (int i = 1; i<map.size.x-1; i++){
		for (int j = 1; j<map.size.y-1; j++){
			// cas 0 -> air
			if (!tab[i*map.size.y + j]){
				tiledMap[i*map.size.y + j] = AIR; // -1 pour l'air
			}
			// cas 1 -> blocks partout autour
			else if (	tab[(i-1)*map.size.y + j-1] && tab[i*map.size.y + j-1] && tab[(i+1)*map.size.y + j-1] &&
						tab[(i-1)*map.size.y + j]   && 							tab[(i+1)*map.size.y + j]   &&
						tab[(i-1)*map.size.y + j+1] && tab[i*map.size.y + j+1] && tab[(i+1)*map.size.y + j+1] ){
				tiledMap[i*map.size.y + j] = FULL ;
			}
			// cas 2
			//else if (                 !tab[i][j-1] &&
			//					tab[i-1][j]   &&                tab[i+1][j] &&
			//											     tab[i][j+1] ){
			//	tiled[i][j] = U_WALL;
			//}
			//// cas 3
			//else if (                 !tab[i][j-1] &&
			//					tab[i-1][j]   &&                tab[i+1][j] &&
			//											    !tab[i][j+1] ){
			//	tiled[i][j] = 3;
			//}
			//// cas 4
			//else if (                 !tab[i][j-1] &&
			//					tab[i-1][j]   &&                tab[i+1][j] &&
			//					                 tab[i][j+1] ){
			//	tiled[i][j] = 4;
			
			// 20 cas ...
		}
	}

	*/
}
