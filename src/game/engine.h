#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "render_system.h"

typedef enum {
	SCREEN_TITLE = 0,
	SCREEN_LOADING,
	SCREEN_GAMEPLAY
} CurrSceneType;

typedef struct {
	const char* dataPath;
	const char* levelsPath;
	short renderWidth;
	short renderHeight;
} EngineSettings;

typedef struct Engine {
	CurrSceneType currSceneType;
	GameRenderer renderer;
	EngineSettings settings;
	int paused;
} Engine;

#endif