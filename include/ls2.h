#ifndef LS2_H_DEFINED
#define LS2_H_DEFINED

#include "dynamic_string.h"
#include "file_stack.h"

/*
 * Given a directory path, print the contents of that directory to stdout
 * Optionally, only print contents with a name matching a specified pattern
 *
 * @arg char* <path> - the directory path
 * @arg nullable const char* <exact_pattern_match> - the name pattern to search for
*/
void ls2(const char *path, const char *exact_pattern_match);

/*
 * Given a directory path, read the contents of that directory, storing their information on a stack
 * Optionally, only store files with a name matching the specified pattern
 * Recursively search all subdirectories
 *
 * @arg FileStack* <stack> - the stack to which file information should be added
 * @arg DynamicString* <path> - the relative location of the current directory
 * @arg nullable const char* <exact_pattern_match> - the name pattern to search for
 * @arg size_t <depth> - how deeply nested is the current directory relative to the starting directory
*/
void r_ls2(FileStack *stack, DynamicString *path, const char *exact_patter_match, size_t depth);

#endif
