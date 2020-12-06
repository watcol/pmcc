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

void initLexer();
int mark();
void jump(int c);
void unmark(int c);
void skipSpace();
char lexCh();
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
int lexOp();
int expOp();
int thisOp(int o);
int theseOp(int *os, int c);
int expThisOp(int o);
int expTheseOp(int *os, int c);

// parse.c
void parse();

// gen.c
void initCode();
void funcBegin(char* name, int ret, int* args, int argc);
void funcEnd();
int lVarFind(char* cur, int len);
int lVarAdd(char* buf, int ty);
int lVarType(int id);
int lVarGet(char* cur, int len, int ty);
int constNum(int ty, int val);
void pushl();
void pushm(int val);
void pushn(int val);
int inc(int var);
int dec(int var);
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
void ret(int var);
int ifBegin();
void ifElse(int id);
void ifEnd(int id);
int whileBegin();
void whileEval(int id);
void whileEnd(int id);

// gen_llvm.c
#define TY_UNKNOWN 0
#define TY_I8 1
#define TY_I16 2
#define TY_I32 3
#define TY_I64 4
#define TY_I8_REF 5
#define TY_I16_REF 6
#define TY_I32_REF 7
#define TY_I64_REF 8
#define TY_I8_REF_REF 9
#define TY_I16_REF_REF 10
#define TY_I32_REF_REF 11
#define TY_I64_REF_REF 12

void llPutTy(int ty);
int llRefTy(int ty);
int llDerefTy(int ty);
void llFuncBegin(char* name, int ret, int* args, int argc);
void llFuncEnd();
void llBbBegin(char* name);
void llBbEnd();
void llInstV(char* name, int var);
void llInstNVAsg(char* name, int dst, int src1, int src2);
void llAlloca(int var);
void llStoreVal(int var, int val);
void llLoad(int dst, int src);

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
int read(void *buf, int count);
void write(void *buf, int count);
void eWrite(void *buf, int count);

#endif
