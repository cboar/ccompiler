#include <stdlib.h>
#include <stdio.h>
#include "util.h"

int safe_to_free(void* ptr, void** freed)
{
	if(ptr == NULL)
		return 0;
	size_t i;
	for(i = 0; freed[i]; i++)
		if(freed[i] == ptr)
			return 0;
	freed[i] = ptr;
	return 1;
}

char get_escaped(char c)
{
	switch(c){
		case 'a': return '\a';
		case 'b': return '\b';
		case 'f': return '\f';
		case 'n': return '\n';
		case 'r': return '\r';
		case 't': return '\t';
		case 'v': return '\v';
		case 'x':
			fprintf(stderr, "Unsupported hex escape!\n");
		default: return c;
	}
}

char* read_file(char* name)
{
	FILE* file = fopen(name, "rb");

	fseek(file, 0, SEEK_END);
	long length = ftell(file);
	rewind(file);

	char* buffer = malloc((length + 1) * sizeof(*buffer));
	fread(buffer, 1, length, file);
	fclose(file);
	buffer[length] = 0;

	return buffer;
}
