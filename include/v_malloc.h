/*
 * VALID MALLOC
 *
 * Not to be confused with linuxs vmalloc (virtual memory allocator)
 */

#ifndef V_MALLOC_H_DEFINED
#define V_MALLOC_H_DEFINED

#include <stdlib.h>

#define V_MALLOC(size) vMalloc(size, __LINE__, __FILE__, __func__);

void* vMalloc(size_t size, size_t line, const char *file_name, const char *func_name);

#endif
