#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tokenizer.h"
#include "dfa.h"

int main(void){
	Token* list = malloc(256 * sizeof(Token));
	tokenize("#include \"stdio.h\"\n\nint main(void){\n\tfor(int i = 0; i < 500; i++){\nprintf(\"hi\");\n\t}\n}\n", 0);

}
