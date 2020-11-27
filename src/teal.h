#ifndef _TEAL_H_
#define _TEAL_H_

// lex.c
#define OP_UNKNOWN 0
#define OP_ADD 1
#define OP_SUB 2
#define OP_MUL 3
#define OP_DIV 4
#define OP_L   5
#define OP_LE  6
#define OP_ME  7
#define OP_M   8
#define OP_E   9
#define OP_NE 10

void init_lexer(char *buf);
void lex_put();
void skip_space();
char lex_ch();
char this_ch(char c);
char exp_this_ch(char c);
int lex_num();
int exp_num();
int at_eof();
void exp_eof();
int lex_op();
int exp_op();
int this_op(int o);
int these_op(int* os, int c);
int exp_this_op(int o);
int exp_these_op(int* os, int c);

// parse.c
void parse(char* buf);

// gen.c
#define VAL_UNKNOWN 0
#define VAL_LEX 1
#define VAL_RAX 2
#define VAL_RDI 3
#define VAL_RSI 4
#define VAL_RDX 5
#define VAL_RCX 6
#define VAL_R8  7
#define VAL_R9  8
#define VAL_AL  9

void init_code();
void func(char* name);
void inst(char* in);
void instv(char* in, int val);
void insts(char* in, char* val);
void instvv(char* in, int dst, int src);
void instvs(char* in, int dst, char* src);

// util.c
#define NULL 0

int is_space(char c);
int is_digit(char c);
int is_alpha(char c);
int is_alphanum(char c);
int length(char *buf);
int digitlen(char *buf);
int identlen(char *buf);
void read_stdin(char *buf, int count);
int ended();
void putc(char c);
void eputc(char c);
void put(char *s);
void eput(char *s);
void puts(char *s);
void eputs(char *s);

// sys.s
void sys_exit(int code);
int read(void *buf, int count);
void write(void *buf, int count);
void ewrite(void *buf, int count);

#endif
