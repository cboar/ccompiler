#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tokenizer.h"
#include "dfa.h"

char* debug_names[] = {
	"String", "Character", "Keyword",
	"Preproc", "Identifier", "Integer",
	"Operator",	"Semicolon", "Container",
	"Whitespace", "Error"
};

int main(void){
	char src[2048];
	FILE* file;
	file = fopen("testcode", "r");
	int res = fscanf(file, "%[\x1-\x7F]", src);

	Token* list = malloc(256 * sizeof(Token));
	int count = tokenize(src, list);

	for(int i = 0; i < count; i++)
		printf("%s:   %s\n", debug_names[list[i].type], list[i].lexeme);

	fclose(file);
}
