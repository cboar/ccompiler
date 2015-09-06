#ifndef PARSER_H
#define PARSER_H

typedef struct Production Production;
struct Production {
	Production* prods;
	int data;
};

void build_productions();

#endif /* PARSER_H */
