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

typedef struct MachineList {
	Machine* machines;
	size_t amt;
} MachineList;

Token* tokenize(char*);
void print_tokenlist(Token*);

#endif /* LEXER_H */
