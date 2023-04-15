#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>
#include "SDL.h"
#include "cglm.h"

typedef struct PlayerData {
	vec2 startingGridXY;
	vec2 currGridXY;
} GamePlayer;

void processGameplayKeyboardInput(const uint8_t keyboardInput);
void processGameplayMouseInput(SDL_Event* e);

#endif