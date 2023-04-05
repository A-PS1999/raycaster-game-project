#include "shader_handling.h"
#include <stdlib.h>
#include <stdio.h>

static inline int programUniformCompare(void* a, void* b) {
	const GLUniformData* uniformA = a;
	const GLUniformData* uniformB = b;
	return strncmp(uniformA->name, uniformB->name, UNIFORM_NAME_MAX_LEN);
}

void initShaderProgram(ShaderProgram* program, const char* vertShaderPath, const char* fragShaderPath) {
	program->programId = glCreateProgram();
	program->linked = 0;
	program->uniformCount = 0;

	readShaderFile(program, TYPE_VERTEX, vertShaderPath);
	readShaderFile(program, TYPE_FRAGMENT, fragShaderPath);

	attachShaderProgram(program, TYPE_VERTEX);
	attachShaderProgram(program, TYPE_FRAGMENT);
}

int readShaderFile(ShaderProgram* program, ShaderType type, const char* path) {
	FILE* file;

	if ((file = fopen(path, "r") == NULL)) {
		return 0;
	}

	fseek(file, 0, SEEK_END);
	size_t file_length = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* shaderData = calloc(file_length + 1, sizeof(char));
	if (!shaderData) {
		fclose(file);
		return 0;
	}

	fread(shaderData, file_length, 1, file);
	fclose(file);

	int compiled = compileShader(program, type, shaderData);
	free(shaderData);
	return compiled;
}

int compileShader(ShaderProgram* program, ShaderType type, const char* shaderData) {
	GLShader* shader = program->shaders + type;

	shader->type = type;

	GLenum glShaderType = 0;

	switch (type) {
		case (TYPE_VERTEX):
			glShaderType = GL_VERTEX_SHADER;
			break;
		case (TYPE_FRAGMENT):
			glShaderType = GL_FRAGMENT_SHADER;
			break;
		default:
			break;
	}

	shader->shaderId = glCreateShader(glShaderType);

	glShaderSource(shader->shaderId, 1, &shaderData, NULL);

	glCompileShader(shader->shaderId);

	int compilationStatus;
	glGetShaderiv(shader->shaderId, GL_COMPILE_STATUS, &compilationStatus);

	if (compilationStatus == 0) {
		glDeleteShader(shader->shaderId);
		return 0;
	}

	return 1;
}

int linkShaderProgram(ShaderProgram* program) {
	GLint linkStatus;

	glLinkProgram(program->programId);

	glGetProgramiv(program->programId, GL_LINK_STATUS, &linkStatus);

	if (linkStatus == 0) {
		program->linked = 0;
		return 0;
	}

	GLint uniformCount;
	GLint maxUniformLen;

	glGetProgramiv(program->programId, GL_ACTIVE_UNIFORMS, &uniformCount);
	glgetProgramiv(program->programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLen);

	program->uniformCount = uniformCount;

	for (int i = 0; i < uniformCount; ++i) {
		GLUniformData* uniform = program->uniforms + i;

		GLint size;
		GLenum type;
		glGetActiveUniform(program->programId, i, maxUniformLen, NULL, &size, &type, uniform->name);

		uniform->location = glGetUniformLocation(program->programId, uniform->name);
		uniform->type = type;
	}

	qsort(program->uniforms, program->uniformCount, sizeof(GLUniformData), programUniformCompare);

	program->linked = 1;

	return 1;
}

void attachShaderProgram(ShaderProgram* program, ShaderType type) {
	glAttachShader(program->programId, program->shaders[type].shaderId);
	program->attachedTrueFalse[type] = 1;
}

void deleteShaderProgram(ShaderProgram* program) {
	for (int i = 0; i < MAX_SHADERS_PER_PROG; ++i) {
		if (program->attachedTrueFalse[i]) {
			glDeleteShader(program->shaders[i].shaderId);
		}
	}

	glDeleteProgram(program->programId);
}