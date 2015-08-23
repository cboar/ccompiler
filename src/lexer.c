#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "regex.h"
#include "lexer.h"
#include "util.h"

Machine* build_machines(size_t* amt_out)
{
	RegexVar rctx[] = {
		#include "spec/regexdefs.txt"
	};
	Token defs[] = {
		#include "spec/tokendefs.txt"
	};
	size_t amt = (sizeof(defs) / sizeof(*defs));
	*amt_out = amt;

	Machine* ms = malloc(sizeof(Machine) * amt);
	for(size_t i = 0; i < amt; i++){
		int** machine = regex(defs[i].lexeme, rctx);
		TokenType type = defs[i].type;
		ms[i] = (Machine){ machine, type, 0, 0, 0, 0 };
	}
	return ms;
}

void free_machines(Machine* ms, size_t amt)
{
	for(size_t i = 0; i < amt; i++){
		int** m = ms[i].machine;
		for(size_t k = 0; m[k]; k++)
			free(m[k]);
		free(m);
	}
	free(ms);
}

size_t update_machines(char c, Machine* ms, size_t amt, int* out_index)
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
								  /* state is accepting */
		if(m->length > highest && m->machine[m->state][0]){
			highest = m->length;
			*out_index = i;
		}
	}
	return highest;
}

void push_list(char* lex, size_t len, TokenType type, TokenList* list)
{
	if((list->count + 1) == list->max){
		list->max += 256;
		Token* new = realloc(list->data, list->max * sizeof(Token));
		if(new == NULL)
			EXIT_ERR("CANNOT REALLOC MEMORY FOR TOKENLIST");
		list->data = new;
	}
	char* str = malloc(len + 1);
	memcpy(str, lex - len, len);
	str[len] = '\0';
	list->data[list->count++] = (Token){ str, type };
}

TokenList tokenize(char* input)
{
	TokenList list = { malloc(256 * sizeof(Token)), 256, 0 };

	int last_best = -1;
	size_t ms_amt, last_len = 0;
	Machine* ms = build_machines(&ms_amt);

	for(char c; (c = *input); input++){
		int best = -1;
		size_t len = update_machines(c, ms, ms_amt, &best);

		if(last_len >= len && last_best != -1)
			push_list(input, last_len, (ms + last_best)->type, &list);

		last_len = len, last_best = best;
		for(size_t i = 0; i < ms_amt; i++){
			Machine* m = (ms + i);
			m->last_length = m->length;
			m->last_state = m->state;
		}
	}
	free_machines(ms, ms_amt);
	return list;
}

void print_tokenlist(TokenList list)
{
	static const char* token_names[] = {
		#include "spec/tokentypes_debug.txt"
	};
	for(size_t i = 0; i < list.count; i++){
		switch(list.data[i].type){
		case WHITESPACE: break;
		case LITERAL:
			printf("%s ", list.data[i].lexeme);
			break;
		default:
			printf("%s ", token_names[list.data[i].type]);
		}
	}
	printf("\n");
}
