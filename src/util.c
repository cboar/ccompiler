#include <stdlib.h>
#include <stdio.h>
#include "util.h"

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
