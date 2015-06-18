#ifndef DFA_H
#define DFA_H

typedef struct State State;
typedef struct Sequence Sequence;

struct State {
	char* charlist;
	State* out0;
	State* out1;
};
struct Sequence {
	State* start;
	State* end;
};

int** dfa(char*);
void test_dfa(char*, int**);

#endif
