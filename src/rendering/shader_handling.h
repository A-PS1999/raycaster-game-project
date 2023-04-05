#ifndef SHADER_HANDLING_H
#define SHADER_HANDLING_H

#include <glad/glad.h>

#define UNIFORM_NAME_MAX_LEN 64
#define NUM_UNIFORMS_MAX 12

typedef struct {
	char name[UNIFORM_NAME_MAX_LEN];
	GLint location;
	GLint type;
} GLUniformData;

typedef enum {
	TYPE_VERTEX = 0,
	TYPE_FRAGMENT,
	MAX_SHADERS_PER_PROG
} ShaderType;

typedef struct {
	GLuint shaderId;
	ShaderType type;
} GLShader;

typedef struct {
	int linked;
	int programId;
	GLShader shaders[MAX_SHADERS_PER_PROG];
	int attachedTrueFalse[MAX_SHADERS_PER_PROG];
	int uniformCount;
	GLUniformData uniforms[NUM_UNIFORMS_MAX];
} ShaderProgram;

void initShaderProgram(ShaderProgram* program, const char* vertShaderPath, const char* fragShaderPath);
int readShaderFile(ShaderProgram* program, ShaderType type, const char* path);
int compileShader(ShaderProgram* program, ShaderType type, const char* shaderData);
int linkShaderProgram(ShaderProgram* program);
void attachShaderProgram(ShaderProgram* program, ShaderType type);
void deleteShaderProgram(ShaderProgram* program);

#endif