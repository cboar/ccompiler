#ifndef TOKENIZER_H
#define TOKENIZER_H

typedef enum TokenType {
	#include "spec/tokentypes.txt"
} TokenType;

typedef struct TokenDef {
	char* pattern;
	TokenType type;
} TokenDef;

typedef struct Token {
	char* lexeme;
	TokenType type;
} Token;

typedef struct Machine {
	int** machine;
	TokenType type;
	int state, ostate;
	size_t length, olength;
} Machine;

size_t tokenize(char*, Token*);
void tokenizer_init();

#endif
