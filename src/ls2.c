#include "ls2.h"
#include "dynamic_string.h"
#include "file_stack.h"

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/resource.h>

void ls2(const char *path, const char *exact_pattern_match) 
{
	// create and populate a stat structure with information about <path>
	// if unable to query information about <path>
	// print an error message and exit
	struct stat statbuf;
	if(lstat(path, &statbuf) != 0)
	{
		fprintf(stderr, "ERROR:\n");
		fprintf(stderr, "\tfailed to query information about \"%s\"\n", path);
		fprintf(stderr, "\texiting...\n");
		exit(EXIT_FAILURE);
	}

	// if <statbuf> indicates that the file located at <path> is not a directory
	// print an error message and exit
	if(!S_ISDIR(statbuf.st_mode))
	{
		fprintf(stderr, "ERROR:\n");
		fprintf(stderr, "\t\"%s\" is not a directory\n", path);
		fprintf(stderr, "\texiting...\n");
		exit(EXIT_FAILURE);
	}

	// create and initialize a <FileStack> 
	// used to hold past file information
	FileStack file_stack;
	FileStack_init(&file_stack);

	// create and initialize a <DynamicString> 
	// used to holding the path of the current file
	DynamicString file_path;
	DynamicString_init(&file_path, path);

	// if the provided path ended with '/' and the path was not one character long (i.e. refering to the root directory)
	// truncate the forward slash
	if(file_path.length != 1 && file_path.content[file_path.length] == '/') DynamicString_truncate(&file_path, 1);

	// traverse this directory and all of its subdirectories collecting file information
	r_ls2(&file_stack, &file_path, exact_pattern_match, 0);

	// print the file information stored in <file_stack>
	FileStack_print(&file_stack);

	// free all dynamic memory being used by <file_stack> and <file_path>
	FileStack_free(&file_stack);
	DynamicString_free(&file_path);
}

void r_ls2(FileStack *stack, DynamicString *path, const char *exact_pattern_match, size_t depth)
{
	// attempt to open the directory indicated by <path>
	// if unable to open said directory, return
	DIR *directory = opendir(path->content);
	if(directory == NULL) return;

	while(1)
	{
		// read the next entry from the open directory
		// if the entry is <NULL>, all files have been read, cleanup and exit
		struct dirent *entry = readdir(directory);
		if(entry == NULL)
		{
			closedir(directory);
			return;
		}

		// if the name of the current entry is "." or "..", skip it
		if(strcmp(entry->d_name, ".")  == 0) continue;
		if(strcmp(entry->d_name, "..") == 0) continue;

		// take note of <path>'s current length
		int appended_length = path->length;

		// add the current entries name to the end of path
		DynamicString_append(path, "/");
		DynamicString_append(path, entry->d_name);

		// calculate the difference between <path>'s old and new length
		// this is the length of the entries name
		appended_length = path->length - appended_length;


		// create and populate a stat structure with information about <path>
		// if unable to query information about the current entry, skip it
		struct stat statbuf;
		if(lstat(path->content, &statbuf) != 0) continue;

		// if the current entry is a directory
		if(S_ISDIR(statbuf.st_mode))
		{
			// take note of <stack>'s current size
			size_t pre_recursion_size = stack->size;

			// recurse on the current entry
			r_ls2(stack, path, exact_pattern_match, depth+1);

			// if the size of the stack has changed, push the current entry onto <stack>
			if(stack->size != pre_recursion_size)
			{
				FileStack_push(stack, DIRECTORY_NODE, entry->d_name, depth, statbuf.st_size);
			}
		}

		// if the current entry is a regular file
		else if(S_ISREG(statbuf.st_mode))
		{
			// if <exact_pattern_match> is undefined, or the current entries name matches <exact_pattern_match>
			// push the current entry onto <stack>
			if(exact_pattern_match == NULL || strcmp(entry->d_name, exact_pattern_match) == 0)
			{
				FileStack_push(stack, REGULAR_NODE, entry->d_name, depth, statbuf.st_size);
			}
		}

		// remove the name of the current entry from the end of <path>
		DynamicString_truncate(path, appended_length);
	}
}
