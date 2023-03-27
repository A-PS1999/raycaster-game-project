#ifndef RENDERER_H
#define RENDERER_H

#include "textures.h"

typedef struct Renderer {
	void (*render)(struct Renderer* renderer, const struct Engine* engine);
	void (*shutdown)(struct Renderer* renderer);
	int (*uploadTexture)(struct Renderer* renderer, );
	void* openglContext;
};

#endif