#ifndef UTIL_H
#define UTIL_H
#include "tokenizer.h"

int safe_to_free(void*, void**);
void print_tokenlist(Token*, size_t);
char get_escaped(char);
char* read_file(char*);

#endif /* UTIL_H */
