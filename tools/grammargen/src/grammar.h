#ifndef GRAMMAR_H
#define GRAMMAR_H

typedef struct Spec {
	char* data;
	size_t amt;
} Spec;

typedef struct Production {
	char* name;
	size_t amt;
	Spec* specs;
} Production;

#endif
