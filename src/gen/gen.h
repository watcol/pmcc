#ifndef _GEN_H_
#define _GEN_H_

#include"../teal.h"

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
