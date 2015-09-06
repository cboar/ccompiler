#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "lexer.h"
#include "parser.h"

int main(void)
{
	char* src = read_file("testcode");
	TokenList list = tokenize(src);
	build_productions(list);
}
