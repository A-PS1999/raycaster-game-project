#include "render_system.h"
#include "file_functions.h"

int initRenderer(GameRenderer* renderSystem, struct Engine* engine, Renderer* renderer, short renderWidth, short renderHeight) {
	if (!(renderSystem && renderer)) return 1;

	renderSystem->viewWidth = renderWidth;
	renderSystem->viewHeight = renderHeight;
	renderSystem->renderer = renderer;

	return 0;
}

int loadTextureToRenderer(GameRenderer* renderSystem, int index, const char* path) {
	Texture* tex = renderSystem->textures + index;

	if (path == NULL) {
		renderSystem->renderer->removeTexture(renderSystem->renderer, tex);
		freeTexture(tex);
		return 0;
	}

	char pathToTexture[MAX_PATH_LEN];
	appendFilePath(pathToTexture, getFileDirectory(PATH_ASSETS), path);

	if (loadTexture(tex, pathToTexture)) {
		return renderSystem->renderer->uploadTexture(renderSystem->renderer, tex);
	}
	else return 0;
}

void renderWorld(GameRenderer* renderSystem, const struct Engine* engine) {
	// TODO
}