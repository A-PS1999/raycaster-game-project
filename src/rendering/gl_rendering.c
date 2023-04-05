#include "gl_rendering.h"
#include "shader_handling.h"

#define MAX_SHADER_PROGRAMS 5

typedef struct {
	ShaderProgram programs[5];
} GLStateContext;

void glRenderingShutdown(Renderer* renderer) {
	GLStateContext* contextToShutdown = renderer->openglContext;

	for (int i = 0; i < MAX_SHADER_PROGRAMS; ++i) {
		deleteShaderProgram(contextToShutdown->programs + i);
	}
}

int glRenderingInit(Renderer* renderer, const char* shadersLocation) {
	GLStateContext* context;

	if ((context = malloc(sizeof(GLStateContext)) == NULL)) {
		return 0;
	}

	renderer->openglContext = context;
	renderer->shutdown = glRenderingShutdown;

	// TODO
}