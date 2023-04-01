#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "render_system.h"

typedef struct {
	unsigned int levelNum;
	char** mapArr;
	unsigned int mapWidth;
	unsigned int mapHeight;
} CurrLevel;

typedef enum {
	SCREEN_TITLE = 0,
	SCREEN_LOADING,
	SCREEN_GAMEPLAY
} CurrSceneType;

typedef struct {
	const char* assetsPath;
	const char* levelsPath;
	short renderWidth;
	short renderHeight;
} EngineSettings;

typedef struct Engine {
	CurrSceneType currSceneType;
	CurrLevel currLevel;
	GameRenderer renderer;
	EngineSettings settings;
	int paused;
} Engine;

#endif