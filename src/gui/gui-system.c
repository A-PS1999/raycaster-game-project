#include "gui-system.h"

int guiSystemInit(GuiSystem* guiSys, SDL_Window* window) {

	guiSys->nuklearCtx = nk_sdl_init(window);
	{
		nk_sdl_font_stash_begin(guiSys->fontAtlas);
		// fonts can be put here
		nl_sdl_font_stash_end();
	}
	nk_style_default(guiSys->nuklearCtx);
}

void guiSystemShutdown(void) {
	nk_sdl_shutdown();
}

void guiSystemBeginFrame(GuiSystem* guiSys) {
	nk_input_begin(guiSys->nuklearCtx);
}

void guiSystemProcessEvent(SDL_Event* e) {
	nk_sdl_handle_event(e);
}

void guiSystemEndFrame(void) {
	nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
}