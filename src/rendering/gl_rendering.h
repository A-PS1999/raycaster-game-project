#ifndef GL_RENDERING_H
#define GL_RENDERING_H

#include "engine.h"

int glRenderingInit(Renderer* renderer, const char* shadersLocation);
void glRenderingShutdown(Renderer* renderer);

#endif