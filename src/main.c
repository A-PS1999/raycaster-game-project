#include <stdio.h>
#include <glad/glad.h>
#include "gl_rendering.h"
#include "file_functions.h"

int glWindowWidth = 1000;
int glWindowHeight = 800;

int main(int argc, const char* argv[]) {

	if (SDL_Init(SDL_INIT_VIDEO) == -1) {
		printf("Error: Failed to initialise SDL2\n");
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	SDL_Window *window = NULL;
	SDL_GLContext* context = NULL;

	window = SDL_CreateWindow("Raycaster Game",
							  SDL_WINDOWPOS_CENTERED,
		                      SDL_WINDOWPOS_CENTERED,
		                      glWindowWidth,
		                      glWindowHeight,
		                      SDL_WINDOW_OPENGL);

	context = SDL_GL_CreateContext(window);

	if (!window || !context) {
		printf("Error: Failed to create window and OpenGL context\n");
		return 0;
	}

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		printf("Failed to initialise GLAD\n");
		SDL_GL_DeleteContext(context);
		return 0;
	}

	SDL_GL_SetSwapInterval(1);

	glViewport(0, 0, glWindowWidth, glWindowHeight);

	Renderer openGLRenderer;
	glRenderingInit(&openGLRenderer, "rendering/shaders/");

	EngineSettings engineSettings;
	engineSettings.assetsPath = "assets/";
	engineSettings.levelsPath = "levels/";
	setFileDirectory(PATH_ASSETS, engineSettings.assetsPath);
	setFileDirectory(PATH_LEVELS, engineSettings.levelsPath);
	engineSettings.renderHeight = glWindowHeight;
	engineSettings.renderWidth = glWindowWidth;

	Engine gameEngine;

	initEngine(&gameEngine, &openGLRenderer, &window, engineSettings);

	while (!gameEngine.quit) {
		engineUpdate(&gameEngine);
		engineDoRender(&gameEngine);
		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);

	return 0;
}