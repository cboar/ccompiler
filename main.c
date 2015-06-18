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
	Token* list = malloc(256 * sizeof(Token));
	int count = tokenize("#include \"stdio.h\"\n\nint main(void){\n\tfor(int i = 0; i < 500; i++){\n\t\tprintf(\"hi\");\n\t}\n}\n\n", list);

	for(int i = 0; i < count; i++)
		printf("%s:       %s\n", debug_names[list[i].type], list[i].lexeme);
}
