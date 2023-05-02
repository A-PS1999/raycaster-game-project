#ifndef GUI_SYSTEM_H
#define GUI_SYSTEM_H

#define MAX_VERTEX_MEMORY 0x80000
#define MAX_ELEMENT_MEMORY 0x20000

#include "SDL.h"
#include <nuklear/nuklear-defines.h>
#include <nuklear/nuklear-sdl-gl.h>

typedef struct GuiSystem {
	struct nk_context* nuklearCtx;
	struct nk_font_atlas* fontAtlas;
} GuiSystem;

int guiSystemInit(GuiSystem* guiSys, SDL_Window* window);
void guiSystemShutdown(void);

void guiSystemBeginFrame(GuiSystem* guiSys);
void guiSystemProcessEvent(SDL_Event* e);
void guiSystemEndFrame(void);

#endif