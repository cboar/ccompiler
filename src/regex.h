#ifndef REGEX_H
#define REGEX_H

typedef struct RegexContext RegexContext;
typedef struct State State;
typedef struct Sequence Sequence;

struct RegexContext {
	const char** ids;
	const char** patterns;
};
struct State {
	char* charlist;
	State* out0;
	State* out1;
};
struct Sequence {
	State* start;
	State* end;
};

RegexContext regex_ctx(size_t);
void regex_define(RegexContext*, const char*, const char*);
int** regex(char*, RegexContext*);

#endif
