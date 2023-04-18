#include "menus.h"

void processMenuMouseInput(SDL_Event* e) {
	static int x;
	static int y;

	x = e->button.x;
	y = e->button.y;
}