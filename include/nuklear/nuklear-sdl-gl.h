#ifndef NUKLEAR_SDL_GL_H
#define NUKLEAR_SDL_GL_H

#include "SDL.h"
#include "nuklear-defines.h"

NK_API struct nk_context* nk_sdl_init(SDL_Window *window);
NK_API void nk_sdl_font_stash_begin(struct nk_font_atlas** atlas);
NK_API void nl_sdl_font_stash_end(void);
NK_API int nk_sdl_handle_event(SDL_Event *e);
NK_API void nk_sdl_render(enum nk_anti_aliasing, int max_vertex_buffer, int max_element_buffer);
NK_API void nk_sdl_shutdown(void);
NK_API void nk_sdl_device_destroy(void);
NK_API void nk_sdl_device_create(void);

#endif