#ifndef LEXER_H
#define LEXER_H

typedef enum TokenType {
	#include "spec/tokentypes.txt"
} TokenType;

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

void tokenizer_init();
size_t tokenize(char*, Token*);
void print_tokenlist(Token*, size_t);

#endif /* LEXER_H */


