#include "ls2.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) 
{
	if(argc == 2) ls2(argv[1], NULL);
	else if(argc == 3) ls2(argv[1], argv[2]);
	else
	{
		fprintf(stderr, "%s <path> [exact_pattern_match]\n", argv[0]);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
