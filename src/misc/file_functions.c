#include "file_functions.h"
#include <string.h>

void setFileDirectory(FileDirectory directory, const char* path) {
	strncpy(fileDirectories[directory], path, MAX_PATH_LEN);
}

char* getFileDirectory(FileDirectory directory) {
	return fileDirectories[directory];
}

void appendFilePath(char* dest, const char* base, const char* to_append) {

	size_t baseStrLen = strnlen(base, MAX_PATH_LEN);

	int baseLastCharIsSlash = (baseStrLen > 0 && base[baseStrLen-1] == '/');
	int appendStartsWithSlash = (to_append[0] == '/');

	const char* appendFormat = (baseLastCharIsSlash || appendStartsWithSlash) ? "%s%s" : "%s/%s";

	snprintf(dest, MAX_PATH_LEN, appendFormat, base, to_append);
}