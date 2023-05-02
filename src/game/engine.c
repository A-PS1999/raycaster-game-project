#include "engine.h"

int initEngine(Engine* engine, Renderer* renderer, SDL_Window* window, EngineSettings settings) {
	if (!renderer) {
		printf("Error: no renderer provided!\n");
		return 0;
	}

	initGameRenderer(&engine->renderer, engine, renderer, settings.renderWidth, settings.renderHeight);
	guiSystemInit(&engine->guiSystem, window);

	engine->paused = 0;
	engine->quit = 0;
	engine->currScreenType = SCREEN_MENU;

	return 1;
}