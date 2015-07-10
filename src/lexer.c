#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "regex.h"
#include "lexer.h"

static Machine* machines;
static size_t amt;

void tokenizer_init()
{
	RegexVar rctx[] = {
		#include "spec/regexdefs.txt"
	};
	Token defs[] = {
		#include "spec/tokendefs.txt"
	};
	amt = (sizeof(defs) / sizeof(*defs));
	machines = malloc(amt * sizeof(*machines));
	for(size_t i = 0; i < amt; i++){
		int** machine = regex(defs[i].lexeme, rctx);
		TokenType type = defs[i].type;
		machines[i] = (Machine){ machine, type, 0, 0, 0, 0 };
	}
}

size_t update_machines(char c)
{
	size_t highest = 0;
	for(size_t i = 0; i < amt; i++){
		Machine* m = (machines + i);
		m->state = m->machine[m->state][c];
		if(m->state == 0){
			m->state = m->machine[0][c];
			m->length = (m->state != 0);
		} else {
			m->length++;
		}
		highest = (m->length > highest) ? m->length : highest;
	}
	return highest;
}

size_t tokenize(char* input, Token* list)
{
	size_t count = 0, ohigh = 0;
	for(char c; (c = *input); input++){
		size_t high = update_machines(c);
		if(ohigh >= high){
			for(size_t i = 0; i < amt; i++){
				Machine* m = (machines + i);
				if(m->olength != ohigh || m->machine[m->ostate][0] != 1)
					continue;
				char* str = malloc(ohigh + 1);
				memcpy(str, input - ohigh, ohigh);
				str[ohigh] = 0;
				list[count++] = (Token){ str, m->type };
				break;
			}
		}
		ohigh = high;
		for(size_t i = 0; i < amt; i++){
			Machine* m = (machines + i);
			m->olength = m->length;
			m->ostate = m->state;
		}
	}
	return count;
}

void print_tokenlist(Token* list, size_t count)
{
	static const char* token_names[] = {
		#include "spec/tokentypes_debug.txt"
	};
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
