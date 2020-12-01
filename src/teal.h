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
#define REG_UNKNOWN 0
#define REG_AL 1
#define REG_DIL 2
#define REG_SIL 3
#define REG_DL 4
#define REG_CL 5
#define REG_R8B 6
#define REG_R9B 7
#define REG_SPL 8
#define REG_BPL 9
#define REG_BL 10
#define REG_AX 11
#define REG_DI 12
#define REG_SI 13
#define REG_DX 14
#define REG_CX 15
#define REG_R8W 16
#define REG_R9W 17
#define REG_SP 18
#define REG_BP 19
#define REG_BX 20
#define REG_EAX 21
#define REG_EDI 22
#define REG_ESI 23
#define REG_EDX 24
#define REG_ECX 25
#define REG_R8D 26
#define REG_R9D 27
#define REG_ESP 28
#define REG_EBP 29
#define REG_EBX 30
#define REG_RAX 31
#define REG_RDI 32
#define REG_RSI 33
#define REG_RDX 34
#define REG_RCX 35
#define REG_R8 36
#define REG_R9 37
#define REG_RSP 38
#define REG_RBP 39
#define REG_RBX 40

int conv_type(int reg, int type);
void inst(char *in);
void instr(char *in, int val);
void instm(char *in, int val);
void instl(char *in);
void instn(char *in, int val);
void instrr(char *in, int dst, int src);
void instrm(char *in, int dst, int src);
void instrl(char *in, int dst);
void instrn(char *in, int dst, int src);
void instmr(char *in, int dst, int src);
void instmm(char *in, int dst, int src);
void instml(char *in, int dst);
void instmn(char *in, int dst, int src);

// gen2.c
#define TY_UNKNOWN 0
#define TY_CHAR 1
#define TY_SHORT 2
#define TY_INT 4
#define TY_LONG 8

void init_code();
int get_varid(char *cur, int len, int type);
int get_type(int id);
int get_offset(int id);
void func(char *name);
void func_fin();
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
void asg(int type);
void addasg(int type);
void subasg(int type);
void mulasg(int type);
void divasg(int type);
void remasg(int type);
void ret();

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
void putnum();
void eputnum();

// sys.s
void sys_exit(int code);
int read(void *buf, int count);
void write(void *buf, int count);
void ewrite(void *buf, int count);

#endif
