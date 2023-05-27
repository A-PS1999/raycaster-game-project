#include "maps.h"
#include "engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int loadMapFromLvl(const char* mapPath, struct Engine* engine) {

	FILE* filePointer = fopen(mapPath, "r");

	if (!filePointer) return 0;

	char lineBuffer[1024];

	unsigned int lvlNum = 0;
	unsigned int width = 0;
	unsigned int height = 0;
	int startX = 0;
	int startY = 0;
	float startRot = 0.0;

	while (lineBuffer[0] != "#") {
		fgets(lineBuffer, 1024, filePointer);

		char* strPointer;
		char* label = strtok_r(lineBuffer, ":", &strPointer);
		char* data = strtok_r(NULL, ": ", &strPointer);

		if (strstr(label, "lvl")) {
			lvlNum = (unsigned int)atoi(data);
		}
		else if (strstr(label, "w")) {
			width = (unsigned int)atoi(data);
		}
		else if (strstr(label, "h")) {
			height = (unsigned int)atoi(data);
		}
		else if (strstr(label, "x")) {
			startX = atoi(data);
		}
		else if (strstr(label, "y")) {
			startY = atoi(data);
		}
		else if (strstr(label, "start_rot")) {
			startRot = atof(data);
		}
	}

	engine->currMap.levelNum = lvlNum;
	engine->currMap.mapWidth = width;
	engine->currMap.mapHeight = height;
	engine->currMap.mapArr = calloc(width * height, sizeof(engine->currMap.mapArr[0]));
	engine->currMap.playerStartPos[0] = startX;
	engine->currMap.playerStartPos[1] = startY;
	engine->currMap.playerStartRot = startRot;

	int currRow = 0;

	while (!feof(filePointer)) {
		fgets(lineBuffer, 1024, filePointer);

		for (int currCol = 0; currCol < width; currCol++) {
			engine->currMap.mapArr[MAP_IDX(engine->currMap.mapWidth, currRow, currCol)] = lineBuffer[currCol];
		}
	}

	fclose(filePointer);

	return 1;
}

void freeMap(struct Engine* engine) {
	free(engine->currMap.mapArr);
	engine->currMap.levelNum = 0;
	engine->currMap.mapArr = NULL;
	engine->currMap.mapHeight = 0;
	engine->currMap.mapWidth = 0;
	engine->currMap.playerStartPos[0] = 0;
	engine->currMap.playerStartPos[1] = 0;
	engine->currMap.playerStartRot = 0.0;
}