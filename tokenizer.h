#ifndef TOKENIZER_H
#define TOKENIZER_H

typedef struct Token {
	char* lexeme;
	int type;
} Token;

size_t tokenize(char*, Token*);
void tokenizer_init();

#endif
