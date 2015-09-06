#include <stddef.h>
#include <stdio.h>
#include "lexer.h"
#include "parser.h"

void build_productions(TokenList tl)
{
	/* Char literal: negative ascii *
	 *        Token: typeindex + 1  *
	 *   Production: 0              */
	Production C[128], T[130];
	for(int i = 0; i < 128; i++){
		if(i < 107)
			T[i] = (Production){ NULL, i + 1 };
		C[i] = (Production){ NULL, -i };
	}

	#include "spec/grammargen.txt"

}
