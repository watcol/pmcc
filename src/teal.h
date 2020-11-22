#ifndef _TEAL_H_
#define _TEAL_H_

#include <stddef.h>

// util.c
void read_stdin(char *buf, size_t count);

// lex.c
#define LEX_EOF 0
#define LEX_SYMBOL 1
#define LEX_NUM 2

void buf2tokens(char *buf, int *tokens, char **start, size_t *len, size_t count);
char *copy_range(char *start, size_t len);

#endif
