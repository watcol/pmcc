#ifndef _GEN_H_
#define _GEN_H_

#include"../teal.h"

// vars.c
void cleanVar();
void addArgs(char** args, int* args_len, int* args_ty, int argc);
int derefVar(int var);
int refVar(int var);
int castVar(int var, int ty);
int lTmpVar(int ty);

// gvars.c
void cleanGVar();

// funcs.c
void cleanFunc();
void declFuncs();

// flow.c
void cleanBb();

// llvm.c
void llPutTy(int ty);
int llIsRef(int ty);
int llIsUnsigned(int ty);
// int llRefTy(int ty);
// int llDerefTy(int ty);
void llFuncDecl(char *name, int len, int ret, int *args, int argc);
void llFuncBegin(char *name, int len, int ret, int *args, int argc);
void llFuncEnd();
void llBb(int id, int bb);
int llAlign(int ty);
void llDefGVar(char* buf, int len, int ty);
void llDefGArray(char* buf, int len, int ty, int c);
void llRetV(int var);
void llRetN(int ty, int val);
void llInstNVAsg(char *name, int dst, int src1, int src2);
void llIcmpNVAsg(char *cond, int dst, int src1, int src2);
void llInstVVAsg(char *name, int dst, int src1, int src2);
void llIcmpVVAsg(char *name, int dst, int src1, int src2);
void llConv(int dst, int src);
void llAlloca(int var);
void llAllocaArray(int var, int ty, int c);
void llArrayToPtr(int dst, int src, int c);
void llGArrayToPtr(int dst, char* buf, int len, int c);
void llStoreVal(int var, int val);
void llStore(int dst, int src);
void llStoreG(int var, char* name, int len);
void llLoad(int dst, int src);
void llFuncCall(char* buf, int len, int ret, int* args, int argc);
void llFuncCallAsg(char* buf, int len, int dst, int* args, int argc);
void llBr(int id, int bb);
void llBrCond(int cond, int id, int bb1, int bb2);


#endif
