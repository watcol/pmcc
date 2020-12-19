#ifndef _GEN_H_
#define _GEN_H_

#include"../teal.h"

// gen
#define BB_UNKNOWN 0
#define BB_IF_BEGIN 1
#define BB_IF_ELSE 2
#define BB_IF_END 3
#define BB_WHILE_EVAL 4
#define BB_WHILE_BEGIN 5
#define BB_WHILE_END 6

void initCode();
int retTy();
void funcDecl(char *name, int len, int ret, int *args, int argc);
void funcBegin(char* name, int len, int ret, char** args, int* arg_lens, int* arg_tys, int argc);
void funcEnd();
int lVarFind(char *cur, int len);
int lVarAdd(char *buf, int len, int ty);
int lVarType(int id);
int defVar(char *buf, int len, int ty);
int constNum(int ty, int val);
int unaryOp(int op, int var);
int binOp(int op, int var1, int var2);
int funcCall(char* buf, int len, int* args, int argc);
void ret(int var);
int ifBegin(int var);
void ifElse(int id);
void ifEnd(int id);
int whileBegin();
void whileEval(int id, int var);
void whileEnd(int id);

// llvm.c
#define TY_UNKNOWN 0
#define TY_VOID 1
#define TY_I1 2
#define TY_U8 3
#define TY_I32 4
#define TY_I1_REF 5
#define TY_U8_REF 6
#define TY_I32_REF 7
#define TY_I1_REF_REF 8
#define TY_U8_REF_REF 9
#define TY_I32_REF_REF 10
#define TY_I1_REF_REF_REF 11
#define TY_U8_REF_REF_REF 12
#define TY_I32_REF_REF_REF 13

void llPutTy(int ty);
int llIsRef(int ty);
int llIsUnsigned(int ty);
int llRefTy(int ty);
int llDerefTy(int ty);
void llFuncDecl(char *name, int len, int ret, int *args, int argc);
void llFuncBegin(char *name, int len, int ret, int *args, int argc);
void llFuncEnd();
void llBb(int id, int bb);
void llInstV(char *name, int var);
void llInstN(char *name, int ty, int val);
void llInstNVAsg(char *name, int dst, int src1, int src2);
void llIcmpNVAsg(char *cond, int dst, int src1, int src2);
void llInstVVAsg(char *name, int dst, int src1, int src2);
void llIcmpVVAsg(char *name, int dst, int src1, int src2);
void llConv(char *name, int dst, int src);
void llAlloca(int var);
void llStoreVal(int var, int val);
void llStore(int dst, int src);
void llLoad(int dst, int src);
void llFuncCall(char* buf, int len, int ret, int* args, int argc);
void llFuncCallAsg(char* buf, int len, int dst, int* args, int argc);
void llBr(int id, int bb);
void llBrCond(int cond, int id, int bb1, int bb2);


#endif
