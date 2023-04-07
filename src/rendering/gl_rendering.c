#include "gl_rendering.h"
#include "shader_handling.h"

#define MAX_SHADER_PROGRAMS 2

enum {
	SHADER_WORLD,
	SHADER_SCREEN_TRIANGLE
} ShaderList;

typedef struct {
	ShaderProgram programs[MAX_SHADER_PROGRAMS];
} GLStateContext;

GLenum getTextureFormat(TexFormat texFormat) {
	GLenum output;

	switch (texFormat) {
		case FORMAT_RGB:
			output = GL_RGB;
			break;
		case FORMAT_RGBA:
			output = GL_RGBA;
			break;
		default:
			output = GL_RGBA;
			break;
	}

	return output;
}

int glUploadTexture(Renderer* renderer, Texture* texture) {
	GLuint currTex;
	glGenTextures(1, &currTex);
	glBindTexture(GL_TEXTURE_2D, currTex);

	GLenum textureFormat = getTextureFormat(texture->texFormat);

	GLenum internalFormat = GL_RGBA;
	if (texture->texFormat == FORMAT_RGB) internalFormat = GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, texture->width, texture->height, 0, internalFormat, GL_UNSIGNED_BYTE, texture->texData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	freeTexture(texture);

	return 1;
}

int initFrameBuffer(Renderer* renderer, short width, short height) {
	glGenFramebuffers(1, &renderer->frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, renderer->frameBuffer);

	glGenTextures(1, &renderer->texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, renderer->texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)width, (GLsizei)height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderer->texColorBuffer, 0);

	glGenRenderbuffers(1, &renderer->renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderer->renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLsizei)width, (GLsizei)height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderer->renderBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Error: framebuffer failed to complete!\n");
		return 0;
	}
	
	// switch back to default
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return 1;
}

void deleteFrameBuffer(struct Renderer* renderer) {
	glDeleteTextures(1, &renderer->texColorBuffer);
	glDeleteRenderbuffers(1, &renderer->renderBuffer);
	glDeleteFramebuffers(1, &renderer->frameBuffer);
}

void glRenderingShutdown(Renderer* renderer) {
	GLStateContext* contextToShutdown = renderer->openglContext;

	for (int i = 0; i < MAX_SHADER_PROGRAMS; ++i) {
		deleteShaderProgram(contextToShutdown->programs + i);
	}

	free(renderer->openglContext);
	renderer->openglContext = NULL;
}

int glRenderingInit(Renderer* renderer, const char* shadersLocation) {
	GLStateContext* context;

	if (context = malloc(sizeof(GLStateContext)) == NULL) {
		return 0;
	}

	renderer->openglContext = context;
	renderer->shutdown = glRenderingShutdown;
	renderer->uploadTexture = glUploadTexture;
	renderer->initFrameBuffer = initFrameBuffer;
	renderer->deleteFrameBuffer = deleteFrameBuffer;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	char vertexShaderPath[MAX_PATH_LEN];
	char fragmentShaderPath[MAX_PATH_LEN];

	// Prep shader program for drawing to fullscreen triangle
	appendFilePath(vertexShaderPath, shadersLocation, "fullscreen-triangle.vert");
	appendFilePath(fragmentShaderPath, shadersLocation, "fullscreen-triangle.frag");

	ShaderProgram* fullScreenTriangle = context->programs + SHADER_SCREEN_TRIANGLE;
	initShaderProgram(fullScreenTriangle, vertexShaderPath, fragmentShaderPath);
	linkShaderProgram(fullScreenTriangle);

	return 1;
}