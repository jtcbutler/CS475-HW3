#ifndef DYNAMIC_STRING_H_DEFINED
#define DYNAMIC_STRING_H_DEFINED

#include <stdlib.h>
#include <stdbool.h>

// how much should a DynamicStrings capacity grow by when resizing
#define SCALING_FACTOR 2

typedef struct
DynamicString
{
	char *content;   // the underlying C string
	size_t length;   // the length of the underlying C string
	size_t capacity; // the size of the buffer used to house the underlying C string
}
DynamicString;

/*
 * Initialize a DynamicString with the contents of standard C string
 *
 * @arg DynamicString* <string> - the DynamicString to initialize
 * @arg const char* <content> - the C string whose content should be copied over
*/
void DynamicString_init(DynamicString *string, const char *content);

/*
 * Append the contents of a standard C string to a DynamicString
 *
 * @arg DynamicString* <string> - the DynamicString to append to
 * @arg const char* <content> - the C string whose content should be appended
*/
void DynamicString_append(DynamicString *string, const char *content);

/*
 * Truncate a specified number of characters from the end of a DynamicString
 *
 * @arg DynamicString* <string> - the DynamicString to truncate
 * @arg size_t <number_of_characters> - the number of characters to truncate
*/
void DynamicString_truncate(DynamicString *string, size_t number_of_characters);

/*
 * Free all dynamic memory currenty being used by a DynamicString and reset its members
 *
 * @arg DynamicString* <string> - the DynamicString to deinitialize
*/
void DynamicString_free(DynamicString *string);

#endif
