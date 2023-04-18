#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>
#include "SDL.h"
#include "cglm.h"

typedef struct PlayerData {
	vec2 currGridXY;
	float rotationSpeed;
	float walkSpeed;
	float playerWidth;
	float playerHeight;
} GamePlayer;

extern GamePlayer player;

void processGameplayKeyboardInput(const uint8_t* keyboardInput, float frameTime);
void processGameplayMouseInput(SDL_Event* e);

#endif