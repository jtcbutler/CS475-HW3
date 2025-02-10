#ifndef FILE_STACK_H_INCLUDED
#define FILE_STACK_H_INCLUDED

#include <stdlib.h>

typedef enum 
FileNodeType
{
	DIRECTORY_NODE,
	REGULAR_NODE
} 
FileNodeType;

typedef struct
FileNode
{
	struct FileNode *next;
	enum FileNodeType type;
	char *name;
	size_t depth;
	size_t size;
}
FileNode;

typedef struct 
FileStack
{
	struct FileNode *top;
	size_t size;
}
FileStack;

void FS_init(FileStack *stack);
void FS_push(FileStack* stack, FileNodeType type, char *name, size_t depth, size_t size);
void FS_print(FileStack* stack);
void FS_free(FileStack* stack);

#endif
