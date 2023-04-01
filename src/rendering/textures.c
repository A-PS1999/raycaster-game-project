#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "textures.h"

static int initTextureStruct(Texture* texture) {
	texture->width = 0;
	texture->height = 0;
	texture->texData = NULL;
	texture->texFormat = FORMAT_NULL
}

int loadTexture(Texture* texture, const char* path) {
	FILE* file;
	int xWidth, yHeight, numChannels;

	if ((file = fopen(path, "rb")) == NULL) {
		printf("Error: failed to open file at path %s", path);
		return 1;
	}

	initTextureStruct(texture);

	stbi_set_flip_vertically_on_load(1);

	unsigned char* data = stbi_load(path, &xWidth, &yHeight, &numChannels, 0);

	if (!data) {
		printf("%s\n", stbi_failure_reason());
		return 1;
	}

	switch (numChannels) {
		case 4:
			texture->texFormat = FORMAT_RGBA;
			break;
		case 3:
			texture->texFormat = FORMAT_RGB;
			break;
	}

	texture->texData = data;
	texture->width = xWidth;
	texture->height = yHeight;

	fclose(file);

	return 0;
}

int freeTexture(Texture* texture) {
	if (texture && texture->texData) {
		free(texture->texData);
		texture->texData = NULL;
	}
}