#include <stdio.h>
#include "util.h"
#include "lexer.h"
#include "tokenizer.h"
#include "dfa.h"

int main(void){
	RegexContext rctx = regex_ctx(32);

	regex_define(&rctx, "TESTDEF", "[0-9]");
	regex("hello{TESTDEF}hey", &rctx);
}
