#include "input_handling.h"

PlayerInputState playerInputState;

void handleMenuInput() {
	SDL_Event e;

	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_QUIT:
				gameEngine.quit = 1;
				break;

			default:
				break;
		}

		
	}
}

void handleGameplayInput() {
	const Uint8* keyboardInput = SDL_GetKeyboardState(NULL);

	SDL_Event e;

	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_QUIT:
				gameEngine.quit = 1;
				break;

			default:
				break;
		}

		processGameplayMouseInput(&e);
	}

	if (gameEngine.currScreenType == SCREEN_GAMEPLAY) {
		processGameplayKeyboardInput(keyboardInput, gameEngine.clock.deltaTime);
	}
}
