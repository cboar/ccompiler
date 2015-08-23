#ifndef UTIL_H
#define UTIL_H

#define EXIT_ERR(msg) do { fprintf(stderr, msg); exit(1); } while(0)

int safe_to_free(void*, void**);
char get_escaped(char);
char* read_file(char*);

#endif /* UTIL_H */
