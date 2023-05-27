#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "render_system.h"
#include "maps.h"
#include "gui-system.h"

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

typedef struct {
	int lastFrameTicks;
	float deltaTime;
} EngineClock;

typedef struct Engine {
	CurrScreenType currScreenType;
	CurrMap currMap;
	GameRenderer renderer;
	GuiSystem guiSystem;
	EngineSettings settings;
	EngineClock clock;
	int paused;
	int quit;
} Engine;

extern Engine gameEngine;

int initEngine(Engine* engine, Renderer* renderer, SDL_Window* window, EngineSettings settings);
void engineUpdate(Engine* engine);
void engineDoRender(Engine* engine);

#endif