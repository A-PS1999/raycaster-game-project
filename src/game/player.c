#include <math.h>
#include "player.h"
#include "input_handling.h"

GamePlayer player = {
	.playerHeight = 1,
	.playerWidth = 1,
	.walkSpeed = 5.0f,
	.rotationSpeed = 30.f
};

void processGameplayKeyboardInput(const uint8_t* keyboardInput, float frameTime) {
	if (keyboardInput[SDL_SCANCODE_E]) {
		double oldPlayerX = playerInputState.playerDir[0];
		playerInputState.playerDir[0] = playerInputState.playerDir[0] * cos(-(player.rotationSpeed * frameTime))
			- playerInputState.playerDir[1] * sin(-(player.rotationSpeed * frameTime));

		playerInputState.playerDir[1] = oldPlayerX * sin(-(player.rotationSpeed * frameTime))
			+ playerInputState.playerDir[1] * cos(-(player.rotationSpeed * frameTime));
	}
}

void processGameplayMouseInput(SDL_Event* e) {

}