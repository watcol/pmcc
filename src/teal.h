#ifndef _TEAL_H_
#define _TEAL_H_

// lex.c
#define OP_ADD 0
#define OP_SUB 1

void init_lexer(char *buf);
void putlen(int len);
void space();
int num();
int exp_num();
int eof();
void exp_eof();
int op();
int exp_op();
int this_op(int o);
void exp_this_op(int o);

// parse.c
void parse(char* buf);

// gen.c
void init_code();
void func(char* name);
void ret();

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
