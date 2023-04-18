#ifndef MENUS_H
#define MENUS_H

#include "SDL.h"

typedef enum {
	MENU_START = 0,
	MENU_BATTLE
} MenuType;

void processMenuMouseInput(SDL_Event* e);

#endif