#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dfa.h"
#include "tokenizer.h"

#define AMT 11

char* debug_names[] = {
	"String", "Character", "Keyword",
	"Preproc", "Id", "Int", "Operator",
	"Semicolon", "Container", "Whitespace",
	"Error"
};
int** machines[AMT], initialized = 0;

void init_tokenizer(){
	machines[0] = dfa("\"[^\"]*\"");
	machines[1] = dfa("'[^']'");
	machines[2] = dfa("auto|break|case|char|const|continue|default|do|double|else|enum|extern|float|for|goto|if|int|long|register|return|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|void|volatile|while");
	machines[3] = dfa("#\\w+");
	machines[4] = dfa("\\w(\\w|\\d)*");
	machines[5] = dfa("\\d+");
	machines[6] = dfa("->|<<=?|>>=?|&&|\\+\\+|--|\\|\\||[\\-+*/!=><%&|^]=?|[~.,?:]");
	machines[7] = dfa(";");
	machines[8] = dfa("[(){}[\\]]");
	machines[9] = dfa("\\s");
	machines[10] = dfa("[^]");
	initialized = 1;
}

void free_machines(){
	for(int i = 0; i < AMT; i++)
		free(machines[i]);
}

size_t tokenize(char* input, Token** tokenlist){
	if(!initialized)
		init_tokenizer();

	int state[AMT] = {0}, nstate[AMT] = {0};
	size_t length[AMT] = {0}, nlength[AMT] = {0};
	size_t oldhighest = 0, count = 0;

	for(char* c = input; *c; c++){
		size_t highest = 0;
		for(size_t m = 0; m < AMT; m++){
			nstate[m] = machines[m][ state[m] ][*c]; //get next state
			if(nstate[m] == 0){ //if state fails
				nstate[m] = machines[m][0][*c]; //reset, try again
				nlength[m] = nstate[m] ? 1 : 0; //if fail, length is 0
			} else {
				nlength[m]++;
			}
			highest = nlength[m] > highest ? nlength[m] : highest;
		}
		if(oldhighest >= highest){ //previous highest no longer matches, add it
			for(size_t m = 0; m < AMT; m++){
				if(length[m] == oldhighest && machines[m][state[m]][0] == 1){ //if is accepting
					if(m != WHITESPACE){
						for(int i = oldhighest; i; i--)
							printf("%c", *(c-i));
						printf("\t\t%s\n", debug_names[m]);
					}
					count++;
					break;
				}
			}
		}
		oldhighest = highest;
		memcpy(state, nstate, AMT * sizeof(*state));
		memcpy(length, nlength, AMT * sizeof(*length));
	}
	return count;
}

#undef AMT
