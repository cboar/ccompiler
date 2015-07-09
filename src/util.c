#include <stdlib.h>
#include <stdio.h>
#include "util.h"

static char* token_names[] = {
	#include "spec/tokentypes_debug.txt"
};

void print_tokenlist(Token* list, size_t count)
{
	for(size_t i = 0; i < count; i++){
		switch(list[i].type){
		case WHITESPACE: break;
		case LITERAL:
			printf("'%s' ", list[i].lexeme);
			break;
		default:
			printf("%s ", token_names[list[i].type]);
		}
	}
	printf("\n");
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
