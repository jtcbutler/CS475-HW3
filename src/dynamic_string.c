#include "dynamic_string.h"
#include "valid_malloc.h"
#include <string.h>

void DynamicString_init(DynamicString *string, const char *content)
{
	// initialize <string>s length and capacity based on the length of <content>
	string->length = strlen(content);
	string->capacity = string->length + 1;

	// dynamically allocate the necessary space
	// copy <content> into <string>
	string->content = VALID_MALLOC(sizeof(char) * string->capacity);
	strcpy(string->content, content);
}

void DynamicString_append(DynamicString *string, const char *new_content)
{
	size_t content_length = strlen(new_content);

	// if <string> does not have enough space to accomadate <new_content>
	// resize <string>
	if(string->length + content_length >= string->capacity)
	{
		// determine a new capacity
		while(string->length + content_length >= string->capacity) string->capacity *= SCALING_FACTOR;

		// allocate a buffer based on the new capacity calculated above
		// copy the old contents into the new buffer
		char *old_content = string->content;
		string->content = VALID_MALLOC(sizeof(char) * string->capacity);
		strcpy(string->content, old_content);

		// free the old buffer
		free(old_content);
	}

	// copy <new_content> into <string> and adjust its size accordingly
	strcat(string->content, new_content);
	string->length += content_length;
}

void DynamicString_truncate(DynamicString *string, size_t number_of_characters)
{
	// add a null terminator at the specified index and adjust size accordingly
	string->content[string->length -= number_of_characters] = '\0';
}

void DynamicString_free(DynamicString *string)
{
	// free the buffer used to hold <string>s contents
	free(string->content);
	string->content = NULL;
	string->length = 0;
	string->capacity = 0;
}
