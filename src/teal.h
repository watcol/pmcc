#ifndef _TEAL_H_
#define _TEAL_H_

#include <stddef.h>

// util.c
void read_stdin(char *buf, size_t count);

// lex.c
#define OP_ADD 0
#define OP_SUB 1

void init_lexer(char *buf);
void space();
int num();
int exp_num();
int eof();
void exp_eof();
int op();
int exp_op();
int this_op(int o);
void exp_this_op(int o);

#endif
