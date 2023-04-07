#ifndef RENDERER_H
#define RENDERER_H

#include "textures.h"

typedef struct Renderer {
	void (*render)(struct Renderer* renderer, const struct Engine* engine);
	void (*shutdown)(struct Renderer* renderer);
	int (*uploadTexture)(struct Renderer* renderer, Texture* texture);
	int (*removeTexture)(struct Renderer* renderer, Texture* texture);
	int (*initFrameBuffer)(struct Renderer* renderer, short width, short height);
	void (*deleteFrameBuffer)(struct Renderer* renderer);
	unsigned int frameBuffer;
	unsigned int texColorBuffer;
	unsigned int renderBuffer;
	void* openglContext;
} Renderer;

#endif