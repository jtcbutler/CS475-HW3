#include "v_malloc.h"
#include <stdio.h>

void* vMalloc(size_t size, size_t line, const char *file_name, const char *func_name)
{
	void *ptr = malloc(size);
	if(!ptr)
	{
		fprintf(stderr, "MEMORY ALLOCATION FAILURE:\n");
		fprintf(stderr, "\tmalloc() failed to allocate a block of size %lu bytes\n", size);
		fprintf(stderr, "\tfailure occured on line %lu of file \"%s\" inside of function %s()\n", line, file_name, func_name);
		fprintf(stderr, "\texiting...\n");
		exit(EXIT_FAILURE);
	}
	return ptr;
}
