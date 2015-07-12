#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "regex.h"
#include "lexer.h"

MachineList build_machines()
{
	RegexVar rctx[] = {
		#include "spec/regexdefs.txt"
	};
	Token defs[] = {
		#include "spec/tokendefs.txt"
	};
	size_t amt = (sizeof(defs) / sizeof(*defs));
	Machine* machines = malloc(amt * sizeof(*machines));
	for(size_t i = 0; i < amt; i++){
		int** machine = regex(defs[i].lexeme, rctx);
		TokenType type = defs[i].type;
		machines[i] = (Machine){ machine, type, 0, 0, 0, 0 };
	}
	return (MachineList){machines, amt};
}

void free_machines(MachineList ml)
{
	for(size_t i = 0; i < ml.amt; i++){
		int** m = ml.machines[i].machine;
		for(size_t k = 0; m[k]; k++)
			free(m[k]);
		free(m);
	}
	free(ml.machines);
}

size_t update_machines(char c, MachineList ml)
{
	Machine* machines = ml.machines;
	size_t highest = 0;
	for(size_t i = 0; i < ml.amt; i++){
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

Token* tokenize(char* input)
{
	Token* list = malloc(2048 * sizeof(*list));
	MachineList ml = build_machines();
	Machine* machines = ml.machines;
	size_t amt = ml.amt;

	size_t count = 0, ohigh = 0;
	for(char c; (c = *input); input++){
		size_t high = update_machines(c, ml);
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
	list[count] = (Token){ NULL };
	free_machines(ml);
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
