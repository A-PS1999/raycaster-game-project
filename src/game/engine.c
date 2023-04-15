#include "engine.h"

int initEngine(Engine* engine, Renderer* renderer, EngineSettings settings) {
	if (!renderer) {
		printf("Error: no renderer provided!\n");
		return 0;
	}

	initGameRenderer(&engine->renderer, engine, renderer, settings.renderWidth, settings.renderHeight);

	engine->paused = 0;
	engine->quit = 0;
	engine->currScreenType = SCREEN_MENU;

	return 1;
}