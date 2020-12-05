#ifndef _TEAL_H_
#define _TEAL_H_

// lex.c
#define OP_UNKNOWN 0
#define OP_ADD 1
#define OP_SUB 2
#define OP_MUL 3
#define OP_DIV 4
#define OP_REM 5
#define OP_LESS 6
#define OP_LEQ 7
#define OP_MORE 8
#define OP_MEQ 9
#define OP_EQ 10
#define OP_NEQ 11
#define OP_NOT 12
#define OP_AND 13
#define OP_OR 14
#define OP_INC 15
#define OP_DEC 16
#define OP_ASG 17
#define OP_ADDASG 18
#define OP_SUBASG 19
#define OP_MULASG 20
#define OP_DIVASG 21
#define OP_REMASG 22

void init_lexer();
int mark();
void jump(int c);
void unmark(int c);
void lex_put();
void skip_space();
char lex_ch();
char this_ch(char c);
char exp_this_ch(char c);
int this_str(char *str);
int exp_this_str(char *str);
int lex_num();
int exp_num();
int lex_ident();
int exp_ident();
int at_eof();
void exp_eof();
int lex_op();
int exp_op();
int this_op(int o);
int these_op(int *os, int c);
int exp_this_op(int o);
int exp_these_op(int *os, int c);

// parse.c
void parse();

// gen1.c
#define TY_UNKNOWN 0
#define TY_I8 1
#define TY_I16 2
#define TY_I32 3
#define TY_I64 4

void putty(int ty);
void func_begin(char* name, int ret, int* args, int argc);
void func_end();
void bb_begin(char* name);
void bb_end();
void instn(char* in, int ty, int val);

// gen2.c
void init_code();
int get_varid(char *cur, int len, int type);
int get_type(int id);
int get_offset(int id);
void pushl();
void pushm(int val);
void pushn(int val);
void inc(int mem);
void dec(int mem);
void add(int type);
void sub(int type);
void mul(int type);
void div(int type);
void rem(int type);
void not_(int type);
void less(int type);
void leq(int type);
void more(int type);
void meq(int type);
void eq(int type);
void neq(int type);
void and_();
void or_();
void asg(int type);
void addasg(int type);
void subasg(int type);
void mulasg(int type);
void divasg(int type);
void remasg(int type);
void ret();
int if_begin();
void if_else(int id);
void if_end(int id);
int while_begin();
void while_eval(int id);
void while_end(int id);

// util.c
#define NULL 0

int is_space(char c);
int is_digit(char c);
int is_alpha(char c);
int is_alphanum(char c);
int length(char *buf);
int str_cmp(char *buf1, char *buf2, int len);
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
void panic(char *msg);
void putnum();
void eputnum();

// sys.s
void sys_exit(int code);
int read(void *buf, int count);
void write(void *buf, int count);
void ewrite(void *buf, int count);

#endif
