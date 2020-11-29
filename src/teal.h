#ifndef _TEAL_H_
#define _TEAL_H_

// lex.c
#define OP_UNKNOWN 0
#define OP_ADD     1
#define OP_SUB     2
#define OP_MUL     3
#define OP_DIV     4
#define OP_REM     5
#define OP_LESS    6
#define OP_LEQ     7
#define OP_MORE    8
#define OP_MEQ     9
#define OP_EQ     10
#define OP_NEQ    11
#define OP_NOT    12
#define OP_AND    13
#define OP_OR     14
#define OP_INC    15
#define OP_DEC    16
#define OP_ASG    17
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
int this_str(char* str);
int exp_this_str(char* str);
int lex_num();
int exp_num();
int lex_ident();
int exp_ident();
int at_eof();
void exp_eof();
int lex_op();
int exp_op();
int this_op(int o);
int these_op(int* os, int c);
int exp_this_op(int o);
int exp_these_op(int* os, int c);

// parse.c
void parse();

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
#define VAL_RSP 9
#define VAL_RBP 10
#define VAL_AL  11

void init_code();
int get_offset(char* cur, int len);
void func(char* name);
void func_fin();
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
int str_cmp(char* buf1, char* buf2, int len);
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
void putnum();
void eputnum();

// sys.s
void sys_exit(int code);
int read(void *buf, int count);
void write(void *buf, int count);
void ewrite(void *buf, int count);

#endif
