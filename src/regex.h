#ifndef REGEX_H
#define REGEX_H

typedef struct State State;
typedef struct Sequence Sequence;

typedef struct RegexVar {
	char *id, *pattern;
} RegexVar;

struct State {
	char *charlist;
	State *out0, *out1;
};

struct Sequence {
	State *start, *end;
};

int** regex(char*, RegexVar*);

#endif /* REGEX_H */
