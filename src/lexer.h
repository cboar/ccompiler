#ifndef LEXER_H
#define LEXER_H

typedef enum TokenType {
	#include "spec/tokentypes.txt"
} TokenType;

typedef struct Token {
	char* lexeme;
	TokenType type;
} Token;

typedef struct TokenList {
	Token* data;
	size_t max, count;
} TokenList;

typedef struct Machine {
	int** machine;
	TokenType type;
	int state, last_state;
	size_t length, last_length;
} Machine;

TokenList tokenize(char*);
void print_tokenlist(TokenList);

#endif /* LEXER_H */
