#ifndef MAPS_H
#define MAPS_H

#define TILE_SIZE 64

typedef struct GameMap {
	unsigned int levelNum;
	char** mapArr;
	unsigned int mapWidth;
	unsigned int mapHeight;
	vec2 playerStartPos;
	vec2 playerStartDir;
} CurrMap;

int loadMapFromLvl(const char* mapPath, struct Engine* engine);
void freeMap(struct Engine* engine);

#endif