#include "file_path.h"
#include "v_malloc.h"
#include <string.h>
#include <stdio.h>

void FP_init(FilePath *path, const char *root)
{
	path->size = strlen(root);

	if(path->size > 1 && root[path->size - 1] == '/') path->size--;

	path->capacity = DEFAULT_FILE_PATH_SIZE;
	while(path->capacity <= path->size) path->capacity *= 2;	

	path->name = V_MALLOC(sizeof(char) * path->capacity);
	for(int i = 0; i < path->size; i++) path->name[i] = root[i];
	path->name[path->size] = '\0';

	path->depth = 0;
}

void FP_descend(FilePath *path, const char *file_name)
{
	size_t file_name_length = strlen(file_name);

	if(path->size + file_name_length + 1 >= path->capacity)
	{
		while(path->size + file_name_length + 1 >= path->capacity) path->capacity *= 2;
		char *old_text = path->name;
		path->name = V_MALLOC(sizeof(char) * path->capacity);
		strcpy(path->name, old_text);
		free(old_text);
		printf("REALOCATING\n");
	}

	path->name[path->size] = '/';
	strcpy(path->name + (sizeof(char) * (path->size + 1)), file_name);
	path->size += file_name_length + 1;
	path->depth++;
}

void FP_ascend(FilePath *path)
{
	if(path->depth == 0) return;

	size_t index = path->size;
	while(path->name[index] != '/') index--;
	path->name[index] = '\0';
	path->size = index;
	path->depth--;
}

void FP_free(FilePath *path)
{
	free(path->name);
}
