#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "dfa.h"

static char SPLIT_EPSILON[1];
static char SINGLE_EPSILON[1];

#define PUSH(s) *sptr++ = s
#define POP() *(--sptr)
#define SEQUENCE() (Sequence){calloc(1,sizeof(State)),calloc(1,sizeof(State))}
#define CONNECT_SPLIT(a,b,c) (*a)=(State){SPLIT_EPSILON,b,c}
#define CONNECT_WITH(a,b,c) (*a)=(State){c,b,0}
#define CONNECT(a,b) (*a)=(State){SINGLE_EPSILON,b,0}

void concat_all(Sequence* stack, Sequence** sptr){
	Sequence ts = {stack->start, ((*sptr)-1)->end};
	while(--(*sptr) != stack)
		CONNECT(((*sptr)-1)->end, (*sptr)->start);
	*(*sptr)++ = ts;
}

Sequence create_nfa(const char* regex){
	Sequence stack[32] = {{0}}, *sptr = stack;
	Sequence s, one, two;
	char *charlist, *tcharlist, tstr[256];
	int i, ti, invert;

	for(const char* c = regex; *c; c++){
		switch(*c){
		case '|':
			concat_all(stack, &sptr);
			one = POP(), s = SEQUENCE();
			for(i = 0; *(c+i); i++)
				tstr[i] = *(c+i+1);
			two = create_nfa(tstr);
			CONNECT(one.end, s.end);
			CONNECT(two.end, s.end);
			CONNECT_SPLIT(s.start, one.start, two.start);
			c += i - 1;
			break;
		case '?':
			one = POP(), s = SEQUENCE();
			CONNECT(one.end, s.end);
			CONNECT_SPLIT(s.start, s.end, one.start);
			break;
		case '+':
			one = POP(), s = SEQUENCE();
			CONNECT(s.start, one.start);
			CONNECT_SPLIT(one.end, one.start, s.end);
			break;
		case '*':
			one = POP(), s = SEQUENCE();
			CONNECT_SPLIT(s.start, s.end, one.start);
			CONNECT_SPLIT(one.end, one.start, s.end);
			break;
		case '.':
			s = create_nfa("[^]");
			break;
		case '[':
			s = SEQUENCE();
			invert = 0, i = 0, charlist = malloc(128 * sizeof(*charlist));
			if(*(c+1) == '^')
				c++, invert = 1;
			while(*(++c) != ']'){
				switch(*c){
				case '-':
					for(char k = *(c-1)+1; k < *(c+1); k++)
						charlist[i++] = k;
					/* fall through */
				case '\\':
					c++;
					/* fall through */
				default:
					charlist[i++] = *c;
				}
			}
			if(invert){
				ti = 0, tcharlist = malloc(128 * sizeof(*tcharlist));
				for(char tc = 1; tc != 127; tc++){
					for(int k = 0; k < i; k++)
						if(charlist[k] == tc)
							goto skip;
					tcharlist[ti++] = tc;
					skip: ;
				}
				charlist = tcharlist, i = ti;
			}
			charlist[i++] = 0;
			tcharlist = realloc(charlist, i * sizeof(*tcharlist));
			CONNECT_WITH(s.start, s.end, tcharlist ? tcharlist : charlist);
			break;
		case '(':
			i = 0;
			for(int count = 1; count > 0; i++){
				c++;
				if(*c == '(')
					count++;
				else if(*c == ')')
					count--;
			}
			memcpy(tstr, c-i+1, i-1);
			tstr[i-1] = 0;
			s = create_nfa(tstr);
			break;
		case '\\':
			c++;
			/* fall through */
		default:
			s = SEQUENCE();
			charlist = malloc(2 * sizeof(*charlist));
			charlist[0] = *c;
			charlist[1] = 0;
			CONNECT_WITH(s.start, s.end, charlist);
		}
		PUSH(s);
	}
	concat_all(stack, &sptr);
	return stack[0];
}
#undef PUSH
#undef POP
#undef SEQUENCE
#undef CONNECT_SPLIT
#undef CONNECT_WITH
#undef CONNECT

size_t get_eclosure_sub(State* s, State*** listptr){
	if(!s)
		return 0;
	*(*listptr)++ = s;
	size_t count = 1;
	if(s->charlist == SPLIT_EPSILON){
		count += get_eclosure_sub(s->out1, listptr);
		count += get_eclosure_sub(s->out0, listptr);
	} else if(s->charlist == SINGLE_EPSILON){
		count += get_eclosure_sub(s->out0, listptr);
	}
	return count;
}

size_t get_eclosure(State* s, State** cache, State*** cacheptrs){
	size_t i;
	for(i = 0; cache[i]; i++)
		if(cache[i] == s)
			return i;
	State** list = malloc(512 * sizeof(State*));
	size_t count = get_eclosure_sub(s, &list);
	*list = NULL;

	State** listn = malloc((count+1) * sizeof(State*));
	memcpy(listn, list-count, (count+1) * sizeof(State*));
	free(list-count);

	cache[i] = s;
	cacheptrs[i] = listn;
	return i;
}

int** create_dfa(Sequence nfa){
	State *cache[512] = {0}, **cacheptrs[512] = {0};
	int i, relinks[512] = {0};

	int** machine = calloc(512, sizeof(int*));
	get_eclosure(nfa.start, cache, cacheptrs);

	for(i = 0; cache[i]; i++){
		machine[i] = calloc(128, sizeof(int));
		for(State** sl = cacheptrs[i], *s = *sl; (s = *sl); sl++){
			int ri = relinks[i] ? relinks[i] : i;
			if(s == nfa.end)
				machine[ri][0] = 1;
			if(!s->charlist || s->charlist == SINGLE_EPSILON || s->charlist == SPLIT_EPSILON)
				continue;
			size_t eclosi = get_eclosure(s->out0, cache, cacheptrs);
			for(char* c = s->charlist; *c; c++){
				if(machine[ri][*c] != 0)
					relinks[eclosi] = machine[ri][*c];
				else
					machine[ri][*c] = eclosi;
			}
		}
	}
	int** nmachine = realloc(machine, i * sizeof(int*));
	return nmachine ? nmachine : machine;
}

int** dfa(char* regex){
	Sequence nfa = create_nfa(regex);
	int** machine = create_dfa(nfa);

	return machine;
}
