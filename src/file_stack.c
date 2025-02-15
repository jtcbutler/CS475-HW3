#include "file_stack.h"
#include "valid_malloc.h"
#include <stdio.h>
#include <string.h>

void FileStack_init(FileStack *stack)
{
	stack->top = NULL;
	stack->size = 0;
}

void FileStack_push(FileStack* stack, FileNodeType type, const char *name, size_t depth, size_t size)
{
	// dynamically allocate a new node structure
	FileNode *node = VALID_MALLOC(sizeof(FileNode));
	
	// initialize the new nodes members with the specified values
	node->next = stack->top;
	node->type = type;
	node->depth = depth;
	node->size = size;

	// allocate space for and copy over the name
	node->name = VALID_MALLOC(sizeof(char) * (strlen(name) + 1));
	strcpy(node->name, name);

	// add the new node to <stack>
	stack->top = node;
	stack->size++;
}

void FileStack_print(FileStack* stack)
{
	FileNode *node = stack->top;

	// iterate through all nodes in the stack
	while(node != NULL)
	{
		// indent <depth> times
		for(int i = 0; i < node->depth; i++) printf("%s", INDENT);

		// select formatting based off of the nodes type
		if(node->type == DIRECTORY_NODE) printf("%s/ (directory)\n", node->name);
		else printf("%s (%lu bytes)\n", node->name, node->size);

		// continue to the next node
		node = node->next;
	}
}

void FileStack_free(FileStack* stack)
{
	// iterate through all nodes in the stack
	while(stack->top != NULL)
	{
		// store the next node to be deleted
		FileNode *node = stack->top;

		// alter the top of the stack
		stack->top = stack->top->next;

		// free the node and its name
		free(node->name);
		free(node);
	}
	stack->size = 0;
}
