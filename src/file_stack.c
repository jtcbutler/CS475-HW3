#include "file_stack.h"
#include "v_malloc.h"
#include "ls2.h"
#include <stdio.h>

void FS_init(FileStack *stack)
{
	stack->top = NULL;
	stack->size = 0;
}

void FS_push(FileStack* stack, FileNodeType type, char *name, size_t depth, size_t size)
{
	FileNode *node = V_MALLOC(sizeof(FileNode));
	
	node->next = stack->top;
	node->type = type;
	node->name = name;
	node->depth = depth;
	node->size = size;

	stack->top = node;
	stack->size++;
}

void FS_print(FileStack* stack)
{
	FileNode *node = stack->top;

	while(node)
	{
		for(int i = 0; i < node->depth; i++) printf("%s", INDENT);

		if(node->type == DIRECTORY_NODE) printf("%s/ (directory)\n", node->name);
		else printf("%s (%lu bytes)\n", node->name, node->size);

		node = node->next;
	}
}

void FS_free(FileStack* stack)
{
	while(stack->size)
	{
		FileNode *node = stack->top;

		stack->top = stack->top->next;
		stack->size--;

		free(node->name);
		free(node);
	}
}
