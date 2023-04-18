#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "render_system.h"
#include "maps.h"

typedef enum {
	SCREEN_MENU = 0,
	SCREEN_GAMEPLAY
} CurrScreenType;

typedef struct {
	const char* assetsPath;
	const char* levelsPath;
	short renderWidth;
	short renderHeight;
} EngineSettings;

typedef struct Engine {
	CurrScreenType currScreenType;
	CurrMap currMap;
	GameRenderer renderer;
	EngineSettings settings;
	int paused;
	int quit;
} Engine;

extern Engine gameEngine;

int initEngine(Engine* engine, Renderer* renderer, EngineSettings settings);

#endif