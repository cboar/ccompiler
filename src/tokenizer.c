#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "regex.h"
#include "tokenizer.h"

/*size_t tokenize(char* input, Token* listptr){
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
					char* str = malloc((oldhighest + 1) * sizeof(char));
					memcpy(str, c - oldhighest, oldhighest * sizeof(char));
					str[oldhighest] = 0;
					*listptr++ = (Token){str, m}; //add token to list
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
}*/
