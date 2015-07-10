#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "lexer.h"
#include "regex.h"

int main(void)
{
	char* src = read_file("testcode");
	Token* list = malloc(256 * sizeof(*list));
	tokenizer_init();
	size_t count = tokenize(src, list);

	print_tokenlist(list, count);
}
