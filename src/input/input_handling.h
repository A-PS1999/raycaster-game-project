#ifndef INPUT_HANDLING_H
#define INPUT_HANDLING_H

#include "engine.h"
#include "player.h"

typedef struct InputState {
	vec2 mousePos;
	vec2 playerPos;
	vec2 playerDir;
	vec2 cameraPlane;
	vec2 strafeDir;
} PlayerInputState;

extern PlayerInputState playerInputState;

void handleMenuInput();

void handleGameplayInput();

#endif