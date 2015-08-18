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
	int state, last_state;
	size_t length, last_length;
} Machine;

Token* tokenize(char*);
void print_tokenlist(Token*);

#endif /* LEXER_H */
