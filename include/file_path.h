#ifndef FILE_PATH_H_INCLUDED
#define FILE_PATH_H_INCLUDED

#include <stdlib.h>

#define DEFAULT_FILE_PATH_SIZE 8

typedef struct
FilePath
{
	char *name;
	size_t size;
	size_t capacity;
	size_t depth;    // the depth of the current filepath relative to the filepath when it was initialized 
}
FilePath;

void FP_init(FilePath *path, const char *file_path);
void FP_descend(FilePath *path, const char *file_name);
void FP_ascend(FilePath *path);
void FP_free(FilePath *path);

#endif
