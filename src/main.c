#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tokenizer.h"
#include "dfa.h"

int main(void){
	char src[2048];
	FILE* file = fopen("testcode", "r");

	if(fscanf(file, "%[\x1-\x7F]", src)){
		tokenizer_init();

	}

	fclose(file);
}
