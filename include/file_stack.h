#ifndef FILE_STACK_H_DEFINED
#define FILE_STACK_H_DEFINED

#include <stdlib.h>

#define INDENT "    "	// indent to be used when printing the stack

typedef enum 
FileNodeType
{
	DIRECTORY_NODE,	// a.k.a  not a regular file, link, or symbolic-link
	REGULAR_NODE	// a.k.a. not a direcory, link, or symbolic-link
} 
FileNodeType;

typedef struct
FileNode
{
	struct FileNode *next;	// node below this one in the stack
	enum FileNodeType type;	// type of file, used for formatting during printing
	char *name;		// name of the file
	size_t depth;			// how deeply nested is this file in comparison to the directory where the stack began
	size_t size;			// file size in bytes
}
FileNode;

typedef struct 
FileStack
{
	struct FileNode *top;	// top of the stack
	size_t size;			// number of nodes in the stack
}
FileStack;

/*
 * Initialize a FileStack
 *
 * @arg FileStack* <stack> - the FileStack to initialize
*/
void FileStack_init(FileStack *stack);

/*
 * Push a new file onto the stack
 *
 * @arg FileStack* <stack> - the FileStack to push onto
 * @arg FileNodeType <type> - the type of the file to push
 * @arg const char* <name> - the name of the file to push
 * @arg size_t <depth> - the depth of the file
 * @arg size_t <size> - the size of the file
*/
void FileStack_push(FileStack* stack, FileNodeType type, const char *name, size_t depth, size_t size);

/*
 * Print the contents of a FileStack
 *
 * @arg FileStack* <stack> - the FileStack to print
*/
void FileStack_print(FileStack* stack);

/*
 * Free all dynamic memory being used by a FileStack and reset its members
 *
 * @arg FileStack* <stack> - the FileStack to deinitialize
*/
void FileStack_free(FileStack* stack);

#endif
