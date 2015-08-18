#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "regex.h"
#include "lexer.h"

size_t build_machines(Machine* out, size_t max)
{
	RegexVar rctx[] = {
		#include "spec/regexdefs.txt"
	};
	Token defs[] = {
		#include "spec/tokendefs.txt"
	};
	size_t amt = (sizeof(defs) / sizeof(*defs));
	if(amt > max){
		fprintf(stderr, "NOT ENOUGH SPACE FOR LEXER MACHINES, EXITING...\n");
		exit(1);
	}

	for(size_t i = 0; i < amt; i++){
		int** machine = regex(defs[i].lexeme, rctx);
		TokenType type = defs[i].type;
		out[i] = (Machine){ machine, type, 0, 0, 0, 0 };
	}
	return amt;
}

void free_machines(Machine* ms, size_t amt)
{
	for(size_t i = 0; i < amt; i++){
		int** m = ms[i].machine;
		for(size_t k = 0; m[k]; k++)
			free(m[k]);
		free(m);
	}
}

size_t update_machines(char c, Machine* ms, size_t amt)
{
	size_t highest = 0;
	for(size_t i = 0; i < amt; i++){
		Machine* m = (ms + i);

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

void reset_machines(Machine* ms, size_t amt)
{
	for(size_t i = 0; i < amt; i++){
		Machine* m = (ms + i);
		m->last_length = m->length;
		m->last_state = m->state;
	}
}

Token* tokenize(char* input)
{
	Token* list = malloc(1024 * sizeof(*list));
	Machine ms[128];
	size_t amt = build_machines(ms, sizeof(ms) / sizeof(*ms));

	size_t count = 0, last_highest = 0;
	for(char c; (c = *input); input++){
		size_t highest = update_machines(c, ms, amt);

		if(last_highest >= highest){
			for(size_t i = 0; i < amt; i++){
				Machine* m = (ms + i);
				if(m->last_length != last_highest || m->machine[m->last_state][0] != 1)
					continue;
				char* str = malloc(last_highest + 1);
				memcpy(str, input - last_highest, last_highest);
				str[last_highest] = 0;
				list[count++] = (Token){ str, m->type };
				break;
			}
		}
		last_highest = highest;
		reset_machines(ms, amt);
	}
	list[count] = (Token){ NULL };

	return list;
}

void print_tokenlist(Token* list)
{
	static const char* token_names[] = {
		#include "spec/tokentypes_debug.txt"
	};
	for(size_t i = 0; list[i].lexeme; i++){
		switch(list[i].type){
		case WHITESPACE: break;
		case LITERAL:
			printf("%s ", list[i].lexeme);
			break;
		default:
			printf("%s ", token_names[list[i].type]);
		}
	}
	printf("\n");
}
