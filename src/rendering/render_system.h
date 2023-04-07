#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#define MAX_TEXTURES 32

#include "renderer.h"

typedef struct {
	short viewWidth;
	short viewHeight;
	Renderer* renderer;
	Texture textures[MAX_TEXTURES];
} GameRenderer;

int initGameRenderer(GameRenderer* renderSystem,
				struct Engine* engine,
				Renderer* renderer,
	            short renderWidth,
	            short renderHeight);

void shutdownGameRenderer(GameRenderer* renderSystem, struct Engine* engine);

int loadTextureToRenderer(GameRenderer* renderSystem, int index, const char* path);

void renderSystemRender(GameRenderer* renderSystem, const struct Engine* engine);

#endif