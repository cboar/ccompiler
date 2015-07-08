#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "regex.h"
#include "tokenizer.h"

void tokenizer_init()
{
	RegexVar rctx[] = {
		{"O", "[0-7]"},
		{"D", "[0-9]"},
		{"NZ", "[1-9]"},
		{"L", "[a-zA-Z_]"},
		{"A", "[a-zA-Z_0-9]"},
		{"H", "[a-fA-F0-9]"},
		{"HP", "0[xX]"},
		{"E", "[Ee][+\\-]?{D}+"},
		{"P", "[Pp][+\\-]?{D}+"},
		{"FS", "f|F|l|L"},
		{"IS", "(u|U)(l|L|ll|LL)?|(l|L|ll|LL)(u|U)?"},
		{"CP", "u|U|L"},
		{"SP", "u8|u|U|L"},
		{"ES", "\\\\(['\"\?\\abfnrtv]|[0-7][0-7]?[0-7]?|x[a-fA-F0-9]+)"},
		{"WS", "[ \t\v\n\f]"},
		{NULL}
	};
	TokenDef tokens[] = {
		{"auto", AUTO},
		{"break", BREAK},
		{"case", CASE},
		{"char", CHAR},
		{"const", CONST},
		{"continue", CONTINUE},
		{"default", DEFAULT},
		{"do", DO},
		{"double", DOUBLE},
		{"else", ELSE},
		{"enum", ENUM},
		{"extern", EXTERN},
		{"float", FLOAT},
		{"for", FOR},
		{"goto", GOTO},
		{"if", IF},
		{"inline", INLINE},
		{"int", INT},
		{"long", LONG},
		{"register", REGISTER},
		{"restrict", RESTRICT},
		{"return", RETURN},
		{"short", SHORT},
		{"signed", SIGNED},
		{"sizeof", SIZEOF},
		{"static", STATIC},
		{"struct", STRUCT},
		{"switch", SWITCH},
		{"typedef", TYPEDEF},
		{"union", UNION},
		{"unsigned", UNSIGNED},
		{"void", VOID},
		{"volatile", VOLATILE},
		{"while", WHILE},
		{"_Alignas", ALIGNAS},
		{"_Alignof", ALIGNOF},
		{"_Atomic", ATOMIC},
		{"_Bool", BOOL},
		{"_Complex", COMPLEX},
		{"_Generic", GENERIC},
		{"_Imaginary", IMAGINARY},
		{"_Noreturn", NORETURN},
		{"_Static_assert", STATIC_ASSERT},
		{"_Thread_local", THREAD_LOCAL},
		{"__func__", FUNC_NAME},
		//{"{L}{A}*", IDENTIFIER},
		{"{HP}{H}+{IS}?", I_CONSTANT},
		{"{NZ}{D}*{IS}?", I_CONSTANT},
		{"0{O}*{IS}?", I_CONSTANT},
		{"{CP}?'([^'\\\\\n]|{ES})+'", I_CONSTANT},
		{"{D}+{E}{FS}?", F_CONSTANT},
		{"{D}*\\.{D}+{E}?{FS}?", F_CONSTANT},
		{"{D}+\\.{E}?{FS}?", F_CONSTANT},
		{"{HP}{H}+{P}{FS}?", F_CONSTANT},
		{"{HP}{H}*\\.{H}+{P}{FS}?", F_CONSTANT},
		{"{HP}{H}+\\.{P}{FS}?", F_CONSTANT},
		{"({SP}?\"([^\"\\\\\n]|{ES})*\"{WS}*)+", STRING_LITERAL},
		{"...", ELLIPSIS},
		{">>=", RIGHT_ASSIGN},
		{"<<=", LEFT_ASSIGN},
		{"+=", ADD_ASSIGN},
		{"-=", SUB_ASSIGN},
		{"*=", MUL_ASSIGN},
		{"/=", DIV_ASSIGN},
		{"%=", MOD_ASSIGN},
		{"&=", AND_ASSIGN},
		{"^=", XOR_ASSIGN},
		{"|=", OR_ASSIGN},
		{">>", RIGHT_OP},
		{"<<", LEFT_OP},
		{"++", INC_OP},
		{"--", DEC_OP},
		{"->", PTR_OP},
		{"&&", AND_OP},
		{"||", OR_OP},
		{"<=", LE_OP},
		{">=", GE_OP},
		{"==", EQ_OP},
		{"!=", NE_OP},
		{"{WS}", WHITESPACE},
		{".", ERROR }
	};

}

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
