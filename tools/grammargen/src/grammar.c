#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "grammar.h"

char* copystr(char* in)
{
	char* out = malloc(strlen(in) + 1);
	strcpy(out, in);
	return out;
}

int readln(char* line, FILE* file)
{
	char* res = fgets(line, 256, file);
	size_t len = strlen(line);
	if(len <= 1)
		return readln(line, file);

	line[len - 1] = '\0';
	return res != NULL;
}

void add_spec(Production* p, char* in){
	char data[256], tmp[256] = {0};
	char* out = calloc(256, 1);
	strcpy(data, in);
	char* items[16];
	size_t iamt = 0;

	items[iamt++] = data;
	for(size_t i = 0; data[i]; i++){
		if(data[i] == ' '){
			data[i] = '\0';
			items[iamt++] = data + i + 1;
		}
	}
	for(size_t i = 0; i < iamt; i++){
		char* curr = items[i];
		if(curr[0] == '\''){
			sprintf(tmp, "C[%s], ", curr);
		} else if(curr[0] >= 'A' && curr[0] <= 'Z'){
			sprintf(tmp, "T[%s], ", curr);
		} else {
			sprintf(tmp, "%s, ", curr);
		}
		strcat(out,tmp);
	}
	size_t len = strlen(out);
	out[len - 2] = '\0';
	p->specs[p->amt++] = (Spec){ out, iamt };
}

int main(void){
	FILE* file = fopen("grammar.txt", "rt");
	char line[256];

	size_t p_count = 0;
	Production current;
	Production prods[256];

	while(readln(line, file)){
		if(line[0] == '\t'){
			switch(line[1]){
			case ':': case '|':
				add_spec(&current, line + 3);
				break;
			case ';':
				prods[p_count++] = current;
				break;
			default:
				fprintf(stderr, "UNKNOWN CHARACTER READ: %d\n", line[1]);
				exit(1);
			}
		} else {
			Spec* specs = calloc(32, sizeof(*specs));
			current = (Production){ copystr(line), 0, specs };
		}
	}
	for(size_t i = 0; i < p_count; i++){
		printf("Production d_%s[%d];\n", prods[i].name, prods[i].amt);
		printf("Production %s = { d_%s, 0 };\n", prods[i].name, prods[i].name);
	}
	printf("\n");
	for(size_t i = 0; i < p_count; i++){
		char* name = prods[i].name;
		for(size_t k = 0; k < prods[i].amt; k++){
			Spec spec = prods[i].specs[k];
			printf("d_%s[%d] = ", name, k);
			if(spec.amt > 1)
				printf("(Production){ (Production[]){ %s }, 0 };\n", spec.data);
			else
				printf("%s;\n", spec.data);
		}
	}
}
