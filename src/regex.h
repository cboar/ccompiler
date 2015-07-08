#ifndef REGEX_H
#define REGEX_H

typedef struct RegexVar RegexVar;
typedef struct State State;
typedef struct Sequence Sequence;

struct RegexVar {
	char *id, *pattern;
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

int** regex(char*, RegexVar*);

#endif
