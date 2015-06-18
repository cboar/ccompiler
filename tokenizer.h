#ifndef TOKENIZER_H
#define TOKENIZER_H

typedef struct Token {
	char* lexeme;
	int type;
} Token;

typedef enum TokenType {
	STRING,
	CHARACTER,
	KEYWORD,
	PREPROCESSOR,
	IDENTIFIER,
	INTEGER,
	OPERATOR,
	SEMICOLON,
	CONTAINER,
	WHITESPACE,
	ERROR
} TokenType;

int tokenize(char*, Token**);
void free_machines();

#endif
