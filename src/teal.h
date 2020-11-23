#ifndef _TEAL_H_
#define _TEAL_H_

// lex.c
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
void putlen(int len);
void space();
char ch();
char this_ch(char c);
char exp_this_ch(char c);
int num();
int exp_num();
int eof();
void exp_eof();
int op();
int exp_op();
int this_op(int o);
int these_op(int* os, int c);
int exp_this_op(int o);
int exp_these_op(int* os, int c);

// parse.c
void parse(char* buf);

// gen.c
#define REG_RAX (-1)
#define REG_RDI (-2)

void init_code();
void func(char* name);
void inst(char* in);
void instr(char* in, int val);
void instv(char* in, char* val);
void instrr(char* in, int dst, int src);
void instrv(char* in, int dst, char* src);

// std.c
#define NULL 0

int is_space(char c);
int is_digit(char c);
int length(char *buf);
int digitlen(char *buf);
void sys_exit(int code);
int read(void *buf, int count);
int ended();
void write(void *buf, int count);
void ewrite(void *buf, int count);
void putc(char c);
void eputc(char c);
void put(char *s);
void eput(char *s);
void puts(char *s);
void eputs(char *s);

// util.c
void read_stdin(char *buf, int count);

// sys.s
int syscall(int num, ...);

#endif
