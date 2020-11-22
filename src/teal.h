#ifndef _TEAL_H_
#define _TEAL_H_

// util.c
void read_stdin(char *buf, int count);

// lex.c
#define OP_ADD 0
#define OP_SUB 1

void init_lexer(char *buf);
void put_len(int len);
void space();
int num();
int exp_num();
int eof();
void exp_eof();
int op();
int exp_op();
int this_op(int o);
void exp_this_op(int o);

// std.c
#define NULL 0

int is_space(char c);
int is_digit(char c);
int length(char *buf);
int digitlen(char *buf);
void sys_exit(int code);
int read(void *buf, int count);
int ended();
int write(void *buf, int count);
int ewrite(void *buf, int count);
int putc(char c);
int eputc(char c);
int put(char *s);
int eput(char *s);
int puts(char *s);
int eputs(char *s);

// sys.s
int syscall(int num, ...);

#endif
