#include "ls2.h"
#include "v_malloc.h"
#include "file_path.h"
#include "file_stack.h"

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

void ls2(const char *path, const char *exact_pattern_match) 
{
	// create and populate a stat structure, <statbuf>
	// if <statbuf> indicates that the file located at <path> is not a directory
	// print an error message and exit
	struct stat statbuf;
	stat(path, &statbuf);
	if(!S_ISDIR(statbuf.st_mode))
	{
		fprintf(stderr, "\"%s\" is not a directory\n", path);
		exit(EXIT_FAILURE);
	}

	// create and initialize a <FileStack> 
	// used to hold the file information during recursive traversal
	FileStack file_stack;
	FS_init(&file_stack);

	// create and initialize a <FilePath> 
	// used for holding the path of the current file during recursive traversal
	FilePath file_path;
	FP_init(&file_path, path);

	// recursively traverse this directory and all of its subdirectories
	r_ls2(&file_stack, &file_path, exact_pattern_match);

	// print the file information stored in <file_stack>
	FS_print(&file_stack);

	// free all dynamic memory being used by <file_stack> and <file_path>
	FS_free(&file_stack);
	FP_free(&file_path);
}

void r_ls2(FileStack *stack, FilePath *path, const char *exact_pattern_match)
{
	// open the directory indicated by <path>
	DIR *directory = opendir(path->name);

	while(1)
	{
		// read the next entry from <directory>
		// if that entry is <NULL> then exit the loop (there are no more entries to read)
		struct dirent *entry = readdir(directory);
		if(entry == NULL) break;

		// ensure that the current entry is not "." or ".."
		// if it is, skip it
		if(strcmp(entry->d_name, ".")  == 0) continue;
		if(strcmp(entry->d_name, "..") == 0) continue;

		// save the current depth and length of <path>
		// these will be used when/if this entry is pushed to <stack>
		int depth = path->depth;
		int length = path->size;

		// append the current entries name to <path>
		FP_descend(path, entry->d_name);

		// create and populate a stat structure, <statbuf> with info about the current entry
		struct stat statbuf;
		stat(path->name, &statbuf);

		// if the current entry is a directory
		if(S_ISDIR(statbuf.st_mode))
		{
			// save the current size of the stack
			size_t pre_recursion_size = stack->size;

			// recurse
			r_ls2(stack, path, exact_pattern_match);

			// if the size of the stack has changed
			// the current entry was not empty and should be included
			if(stack->size != pre_recursion_size)
			{
				// dynamically allocate a copy of the current entries name
				char *name = V_MALLOC(sizeof(char) * (path->size - length));
				strcpy(name, entry->d_name);

				// push the current entry onto <stack>
				FS_push(stack, DIRECTORY_NODE, name, depth, statbuf.st_size);
			}
		}

		// if the current entry is a regular file
		else if(S_ISREG(statbuf.st_mode))
		{
			// if <exact_pattern_match> is not defined or the current entries name matches <exact_pattern_match>
			// push the current entry onto the <stack>
			if(exact_pattern_match == NULL || strcmp(entry->d_name, exact_pattern_match) == 0)
			{
				// dynamically allocate a copy of the current entries name
				char *name = V_MALLOC(sizeof(char) * (path->size - length));
				strcpy(name, entry->d_name);

				// push the current entry onto <stack>
				FS_push(stack, REGULAR_NODE, name, depth, statbuf.st_size);
			}
		}

		// remove the current entries name from the end of <path>
		FP_ascend(path);
	}

	// close the directory indicated by <path>
	closedir(directory);
}
