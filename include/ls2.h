#ifndef LS2_H_INCLUDED
#define LS2_H_INCLUDED

#include "file_path.h"
#include "file_stack.h"

#define INDENT "    "

void ls2(const char *path, const char *exact_pattern_match);
void r_ls2(FileStack *stack, FilePath *path, const char *exact_patter_match);

#endif
