#include "input_handling.h"

int initEngine(Engine* engine, Renderer* renderer, SDL_Window* window, EngineSettings settings) {
	if (!renderer) {
		printf("Error: no renderer provided!\n");
		return 0;
	}

	initGameRenderer(&engine->renderer, engine, renderer, settings.renderWidth, settings.renderHeight);
	guiSystemInit(&engine->guiSystem, window);

	engine->paused = 0;
	engine->quit = 0;
	engine->clock.lastFrameTicks = 0;
	engine->currScreenType = SCREEN_MENU;

	return 1;
}

void engineUpdate(Engine* engine) {
	engine->clock.deltaTime = (SDL_GetTicks() - engine->clock.lastFrameTicks) / 1000.0f;
	engine->clock.lastFrameTicks = SDL_GetTicks();

	if (engine->currScreenType == SCREEN_MENU) {
		handleMenuInput();
	}
	else {
		handleGameplayInput();
	}
}

void engineDoRender(Engine* engine) {
	renderSystemRender(&engine->renderer, engine);
}