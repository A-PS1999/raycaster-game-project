#ifndef FILE_FUNCS_H
#define FILE_FUNCS_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_PATH_LEN 256

typedef enum {
	PATH_ASSETS = 0,
	PATH_LEVELS,
	NUM_FILE_PATHS
} FileDirectory;

extern char fileDirectories[NUM_FILE_PATHS][MAX_PATH_LEN];

void setFileDirectory(FileDirectory directory, const char* path);
char* getFileDirectory(FileDirectory directory);
void appendFilePath(char* dest, const char* base, const char* to_append);

#endif