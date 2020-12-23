#ifndef _TEAL_H_
#define _TEAL_H_

// lex
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
#define OP_DEREF 23
#define OP_REF 24

void initLexer();
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
int thisOp(int o);
int theseOp(int *os, int c);
int expThisOp(int o);
int expTheseOp(int *os, int c);
char expCh();

// parse
#define MAX_ARGS 10

void parse();

// gen
#define TY_UNKNOWN 0
#define TY_VOID 1
#define TY_I1 2
#define TY_U8 3
#define TY_I32 4
#define TY_I64 5
#define TY_I1_REF 6
#define TY_U8_REF 7
#define TY_I32_REF 8
#define TY_I64_REF 9
#define TY_I1_REF_REF 10
#define TY_U8_REF_REF 11
#define TY_I32_REF_REF 12
#define TY_I64_REF_REF 13
#define TY_I1_REF_REF_REF 14
#define TY_U8_REF_REF_REF 15
#define TY_I32_REF_REF_REF 16
#define TY_I64_REF_REF_REF 17

#define BB_UNKNOWN 0
#define BB_IF_BEGIN 1
#define BB_IF_ELSE 2
#define BB_IF_END 3
#define BB_WHILE_EVAL 4
#define BB_WHILE_BEGIN 5
#define BB_WHILE_END 6

#define VAR_VOID (-1)

void initCode();
void finCode();
int retTy();
void funcDecl(char *name, int len, int ret, int *args, int argc);
void funcBegin(char* name, int len, int ret, char** args, int* args_len, int* args_ty, int argc);
void funcEnd();
int lVarFind(char *cur, int len);
int lVarAdd(char *buf, int len, int ty);
int lVarType(int id);
int defVar(char *buf, int len, int ty);
int defArray(char* buf, int len, int ty, int c);
int constNum(int ty, int val);
int unaryOp(int op, int var);
int binOp(int op, int var1, int var2);
int arrayElem(int var, int id);
int funcCall(char* buf, int len, int* args, int argc);
void ret(int var);
int ifBegin(int var);
void ifElse(int id);
void ifEnd(int id);
int whileBegin();
void whileEval(int id, int var);
void whileEnd(int id);

int llRefTy(int ty);
int llDerefTy(int ty);

// util.c
#define NULL 0

int isSpace(char c);
int isDigit(char c);
int isAlpha(char c);
int isAlphaNum(char c);
int length(char *buf);
int strCmp(char *buf1, char *buf2, int len);
int strToI(char *buf, char **ret);
int digitLen(char *buf);
int identLen(char *buf);
void readStdin(char *buf, int count);
int ended();
void putCh(char c);
void ePutCh(char c);
void putStr(char *s);
void ePutStr(char *s);
void putStrLn(char *s);
void ePutStrLn(char *s);
void panic(char *msg);
void putNum();
void ePutNum();

// sys.s
void sysExit(int code);
int read(char *buf, int count);
void write(char *buf, int count);
void eWrite(char *buf, int count);

#endif
