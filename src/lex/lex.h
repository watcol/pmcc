#ifndef _LEX_H_
#define _LEX_H_

#include"../teal.h"
// lex.c
void initLexer();
void panicLex();
char* getCursor();
char thisCh(char c);
char expThisCh(char c);
int thisStr(char *str);
int expThisStr(char *str);
int lexNum();
int expNum();
int lexIdent();
int expIdent();
int atEof();
void expEof();

// op.c
#define OP_UNKNOWN 0
#define OP_ADD 1
#define OP_SUB 2
#define OP_MUL 3
#define OP_DIV 4
#define OP_REM 5
#define OP_LT 6
#define OP_LE 7
#define OP_GT 8
#define OP_GE 9
#define OP_EQ 10
#define OP_NE 11
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

int thisOp(int o);
int theseOp(int *os, int c);
int expThisOp(int o);
int expTheseOp(int *os, int c);

#endif
