#ifndef TEXTURES_H
#define TEXTURES_H

#include "file_functions.h"

typedef enum {
	FORMAT_RGB,
	FORMAT_RGBA,
	FORMAT_NULL
} TexFormat;

typedef struct {
	unsigned int width;
	unsigned int height;
	unsigned char* texData;
	TexFormat texFormat;
} Texture;

int loadTexture(Texture* texture, const char* path);

#endif