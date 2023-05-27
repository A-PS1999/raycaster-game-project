#ifndef MAPS_H
#define MAPS_H

#include "cglm.h"

#define MAP_IDX(WIDTH, ROW, COL) (WIDTH * ROW) + COL
#define TILE_SIZE 64

typedef struct GameMap {
	unsigned int levelNum;
	char* mapArr;
	unsigned int mapWidth;
	unsigned int mapHeight;
	vec2 playerStartPos;
	float playerStartRot;
} CurrMap;

int loadMapFromLvl(const char* mapPath, struct Engine* engine);
void freeMap(struct Engine* engine);

#endif