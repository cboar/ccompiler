#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "dfa.h"

#define push(s) *sptr++ = s
#define pop() *(--sptr)
#define MALLOC(a) *++(*listptr) = malloc(a)
#define CONNECT_SPLIT(a,b,c) (*a)=(State){SPLIT_EPSILON,b,c}
#define CONNECT_WITH(a,b,c) (*a)=(State){c,b,0}
#define CONNECT(a,b) (*a)=(State){SINGLE_EPSILON,b,0}
#define SEQ_ALLOC() s.start=(*++(*listptr)=calloc(1,sizeof(State*))),s.end=(*++(*listptr)=calloc(1,sizeof(State*)));

int *SPLIT_EPSILON, *SINGLE_EPSILON;

void concat_all(Sequence* stack, Sequence** sptr){
	Sequence ts = {stack->start, ((*sptr)-1)->end};
	while(--(*sptr) != stack)
		CONNECT(((*sptr)-1)->end, (*sptr)->start);
	*(*sptr)++ = ts;
}

Sequence create_nfa(char* regex, void*** listptr){
	Sequence stack[128] = {0}, *sptr = stack;
	Sequence s, one, two;
	char tstr[256];
	int i, ni, invert, *charlist, *tcharlist;

	for(char* c = regex; *c; c++){
		if(*c == '\\' && *c++){
			switch(*c){
				case 'd':
					s = create_nfa("[0-9]", listptr);
					break;
				case 'w':
					s = create_nfa("[A-Za-z]", listptr);
					break;
				case 's':
					s = create_nfa("[\t\n ]", listptr);
					break;
				default:
					SEQ_ALLOC();
					charlist = MALLOC(2 * sizeof(int));
					charlist[0] = *c;
					charlist[1] = 0;
					CONNECT_WITH(s.start, s.end, charlist);
			}
		} else {
			switch(*c){
				case '|':
					concat_all(stack, &sptr);
					one = pop(), SEQ_ALLOC();
					for(i = 0; *(c+i); i++)
						tstr[i] = *(c+i+1);
					two = create_nfa(tstr, listptr);
					CONNECT(one.end, s.end);
					CONNECT(two.end, s.end);
					CONNECT_SPLIT(s.start, one.start, two.start);
					c += i - 1;
					break;
				case '?':
					one = pop(), SEQ_ALLOC();
					CONNECT(one.end, s.end);
					CONNECT_SPLIT(s.start, s.end, one.start);
					break;
				case '+':
					one = pop(), SEQ_ALLOC();
					CONNECT(s.start, one.start);
					CONNECT_SPLIT(one.end, one.start, s.end);
					break;
				case '*':
					one = pop(), SEQ_ALLOC();
					CONNECT_SPLIT(s.start, s.end, one.start);
					CONNECT_SPLIT(one.end, one.start, s.end);
					break;
				case '[':
					SEQ_ALLOC();
					invert = 0, i = 0, charlist = MALLOC(128 * sizeof(int));
					if(*(c+1) == '^')
						c++, invert = 1;
					while(*(++c) != ']'){
						switch(*c){
							case '-':
								for(int k = *(c-1)+1; k < *(c+1); k++)
									charlist[i++] = k;
							case '\\':
								c++;
							default:
								charlist[i++] = *c;
						}
					}
					if(invert){
						ni = 0, tcharlist = MALLOC(128 * sizeof(int));
						for(char tc = 1; tc != 127; tc++){
							for(int k = 0; k < i; k++)
								if(charlist[k] == tc)
									goto skip;
							tcharlist[ni++] = tc;
							skip: continue;
						}
						charlist = tcharlist, i = ni;
					}
					charlist[i++] = 0;
					tcharlist = MALLOC(i * sizeof(int));
					memcpy(tcharlist, charlist, i * sizeof(int));
					CONNECT_WITH(s.start, s.end, tcharlist);
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
					s = create_nfa(tstr, listptr);
					break;
				default:
					SEQ_ALLOC();
					charlist = MALLOC(2 * sizeof(int));
					charlist[0] = *c;
					charlist[1] = 0;
					CONNECT_WITH(s.start, s.end, charlist);
			}
		}
		push(s);
	}
	concat_all(stack, &sptr);
	return stack[0];
}
#undef push
#undef pop
#undef MALLOC
#undef CONNECT_SPLIT
#undef CONNECT_WITH
#undef CONNECT
#undef SEQ_ALLOC

int get_eclosure_sub(State* s, State*** listptr){
	if(!s)
		return 0;
	*(*listptr)++ = s;
	int count = 1;
	if(s->charlist == SPLIT_EPSILON){
		count += get_eclosure_sub(s->out1, listptr);
		count += get_eclosure_sub(s->out0, listptr);
	} else if(s->charlist == SINGLE_EPSILON){
		count += get_eclosure_sub(s->out0, listptr);
	}
	return count;
}

int get_eclosure(State* s, State** cache, State*** cacheptrs){
	int i;
	for(i = 0; cache[i]; i++)
		if(cache[i] == s)
			return i;
	State** list = malloc(512 * sizeof(State*));
	int count = get_eclosure_sub(s, &list);
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
			int ri = relinks[i] ?: i;
			if(s == nfa.end)
				machine[ri][0] = 1;
			if(s->charlist != SINGLE_EPSILON && s->charlist != SPLIT_EPSILON){
				int eclosi = get_eclosure(s->out0, cache, cacheptrs);
				printf("%d %d\t(", ri, eclosi);
				for(int* c = s->charlist; *c; c++){
					if(machine[ri][*c] != 0)
						relinks[eclosi] = machine[ri][*c];
					else {
						printf(*c < 32 ? "%d," : "%c", *c);
						machine[ri][*c] = eclosi;
					}
				}
				printf(")\n");
			}
		}
	}
	int** nmachine = malloc(i * sizeof(int*));
	memcpy(nmachine, machine, i * sizeof(int*));
	free(machine);
	return nmachine;
}

int** dfa(char* regex){
	void** listptr = malloc(sizeof(void*) * 1024);
	listptr[0] = 0;

	Sequence nfa = create_nfa(regex, &listptr);
	int** machine = create_dfa(nfa);

	while(*listptr)
		free(*listptr--);

	free(listptr);
	return machine;
}
