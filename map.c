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

void autoTile(Map map, int x, int y){
    // hors de la map
    if (x <= 0 || x >= map.size.x-1 || y <= 0 || y >= map.size.y-1){
        // bords de la map
        if (x == 0 || x == map.size.x-1 || y == 0 || y == map.size.y-1){
            map.tiled[x*map.size.y + y] = FULL_WALL;
        }
    }
    // air
    else if (!map.data[x*map.size.y + y]){
		map.tiled[x*map.size.y + y] = -1; // -1 pour l'air
	}
    // blocks partout autour
	else if (	map.data[(x-1)*map.size.y + y-1] && map.data[x*map.size.y + y-1] && map.data[(x+1)*map.size.y + y-1] &&
				map.data[(x-1)*map.size.y + y]   && 							    map.data[(x+1)*map.size.y + y]   &&
				map.data[(x-1)*map.size.y + y+1] && map.data[x*map.size.y + y+1] && map.data[(x+1)*map.size.y + y+1] ){
		map.tiled[x*map.size.y + y] = FULL_WALL ;
	}
    // U_WALL
	else if (                                       !map.data[x*map.size.y + y-1] && 
				map.data[(x-1)*map.size.y + y]  && 							        map.data[(x+1)*map.size.y + y]   &&
				                                    map.data[x*map.size.y + y+1]  ){
		map.tiled[x*map.size.y + y] = U_WALL;
	}
	// D_WALL
	else if (	                                map.data[x*map.size.y + y-1] && 
				map.data[(x-1)*map.size.y + y] && 							    map.data[(x+1)*map.size.y + y] &&
				                                !map.data[x*map.size.y + y+1] ){
		map.tiled[x*map.size.y + y] = D_WALL;
    }
	// UD_WALL
    else if (	                                !map.data[x*map.size.y + y-1] && 
				map.data[(x-1)*map.size.y + y] && 							    map.data[(x+1)*map.size.y + y] &&
				                                !map.data[x*map.size.y + y+1] ){
		map.tiled[x*map.size.y + y] = UD_WALL;
    }
    // L_WALL
    else if (	                                map.data[x*map.size.y + y-1] && 
				!map.data[(x-1)*map.size.y + y] && 							    map.data[(x+1)*map.size.y + y] &&
				                                map.data[x*map.size.y + y+1] ){
		map.tiled[x*map.size.y + y] = L_WALL;
    }
    // R_WALL
    else if (	                                map.data[x*map.size.y + y-1] && 
				map.data[(x-1)*map.size.y + y] && 							    !map.data[(x+1)*map.size.y + y] &&
				                                map.data[x*map.size.y + y+1] ){
		map.tiled[x*map.size.y + y] = R_WALL;
    }
    // UDL_WALL
    else if (	                                !map.data[x*map.size.y + y-1] && 
				!map.data[(x-1)*map.size.y + y] && 							    map.data[(x+1)*map.size.y + y] &&
				                                !map.data[x*map.size.y + y+1] ){
		map.tiled[x*map.size.y + y] = UDL_WALL;
    }
    // UDR_WALL
    else if (	                                !map.data[x*map.size.y + y-1] && 
				map.data[(x-1)*map.size.y + y] && 							    !map.data[(x+1)*map.size.y + y] &&
				                                !map.data[x*map.size.y + y+1] ){
		map.tiled[x*map.size.y + y] = UDR_WALL;
    }
    // LU_WALL
    else if (	                                !map.data[x*map.size.y + y-1] && 
				!map.data[(x-1)*map.size.y + y] && 							    map.data[(x+1)*map.size.y + y] &&
				                                map.data[x*map.size.y + y+1] ){
		map.tiled[x*map.size.y + y] = LU_WALL;
    }
    // RU_WALL
    else if (	                                !map.data[x*map.size.y + y-1] && 
				map.data[(x-1)*map.size.y + y] && 							    !map.data[(x+1)*map.size.y + y] &&
				                                map.data[x*map.size.y + y+1] ){
		map.tiled[x*map.size.y + y] = RU_WALL;
    }
    // LD_WALL
    else if (	                                map.data[x*map.size.y + y-1] && 
				!map.data[(x-1)*map.size.y + y] && 							    map.data[(x+1)*map.size.y + y] &&
				                                !map.data[x*map.size.y + y+1] ){
		map.tiled[x*map.size.y + y] = LD_WALL;
    }
    // RD_WALL
    else if (	                                map.data[x*map.size.y + y-1] && 
				map.data[(x-1)*map.size.y + y] && 							    !map.data[(x+1)*map.size.y + y] &&
				                                !map.data[x*map.size.y + y+1] ){
		map.tiled[x*map.size.y + y] = RD_WALL;
    }
    // CLU_WALL
    else if (	!map.data[(x-1)*map.size.y + y-1] && map.data[x*map.size.y + y-1] && 
				map.data[(x-1)*map.size.y + y]   && 							    map.data[(x+1)*map.size.y + y] &&
				                                    map.data[x*map.size.y + y+1]  ){
		map.tiled[x*map.size.y + y] = CLU_WALL ;
	}
    // CRU_WALL
    else if (	                                    map.data[x*map.size.y + y-1] && !map.data[(x+1)*map.size.y + y-1] &&
				map.data[(x-1)*map.size.y + y]   && 							    map.data[(x+1)*map.size.y + y]   &&
				                                    map.data[x*map.size.y + y+1] ){
		map.tiled[x*map.size.y + y] = CRU_WALL ;
	}
    // CLD_WALL
    else if (	                                    map.data[x*map.size.y + y-1] &&
				map.data[(x-1)*map.size.y + y]   && 							    map.data[(x+1)*map.size.y + y]   &&
				!map.data[(x-1)*map.size.y + y+1] && map.data[x*map.size.y + y+1] ){
		map.tiled[x*map.size.y + y] = CLD_WALL ;
	}
    // CRD_WALL
    else if (	                                    map.data[x*map.size.y + y-1] && 
				map.data[(x-1)*map.size.y + y]   && 							    map.data[(x+1)*map.size.y + y]   &&
				                                    map.data[x*map.size.y + y+1] && !map.data[(x+1)*map.size.y + y+1] ){
		map.tiled[x*map.size.y + y] = CRD_WALL ;
	}
    // RLU_WALL
    else if (	                                !map.data[x*map.size.y + y-1] && 
				!map.data[(x-1)*map.size.y + y] && 							    !map.data[(x+1)*map.size.y + y] &&
				                                map.data[x*map.size.y + y+1] ){
		map.tiled[x*map.size.y + y] = RLU_WALL;
    }
    // RL_WALL
    else if (	                                map.data[x*map.size.y + y-1] && 
				!map.data[(x-1)*map.size.y + y] && 							    !map.data[(x+1)*map.size.y + y] &&
				                                map.data[x*map.size.y + y+1] ){
		map.tiled[x*map.size.y + y] = RL_WALL;
    }
    // RLD_WALL
    else if (	                                map.data[x*map.size.y + y-1] && 
				!map.data[(x-1)*map.size.y + y] && 							    !map.data[(x+1)*map.size.y + y] &&
				                                !map.data[x*map.size.y + y+1] ){
		map.tiled[x*map.size.y + y] = RLD_WALL;
    }
    // RLUD_WALL
    else if (	                                !map.data[x*map.size.y + y-1] && 
				!map.data[(x-1)*map.size.y + y] && 							    !map.data[(x+1)*map.size.y + y] &&
				                                !map.data[x*map.size.y + y+1] ){
		map.tiled[x*map.size.y + y] = RLUD_WALL;
    }
    else printf("erreur dans autoTile\n");
}

void localAutoTile(Map map, int x, int y){
	for (int i = x-1; i<=x+1; i++){
		for (int j = y-1; j<=y+1; j++){
			autoTile(map, i, j);
		}
	}
}

void completeAutoTile(Map map){
	for (int i = 0; i<map.size.x; i++){
		for (int j = 0; j<map.size.y; j++){
			autoTile(map, i, j);
		}
	}
}
