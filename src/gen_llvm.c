#include"teal.h"

char* types[13] = {
  "unknown",
  "i1",   "i8",   "i32",   "i64",
  "i1*",  "i8*",  "i32*",  "i64*",
  "i1**", "i8**", "i32**", "i64**"
};

void llPutTy(int ty) {
  if(ty == TY_UNKNOWN) {
    panic("Unknown type.");
  }

    putStr(types[ty]);
}

int llRefTy(int ty) {
  if(ty <= TY_UNKNOWN) panic("Unknown type.");
  if(ty >= TY_I8_REF_REF) panic("Can't refer the reference of reference.");

  return ty + 4;
}

int llDerefTy(int ty) {
  if(ty <= TY_UNKNOWN) panic("Unknwon type.");
  if(ty <= TY_I64) panic("Can't derefer the entity type.");

  return ty - 4;
}

void llFuncBegin(char* name, int ret, int* args, int argc) {
  putStr("define ");
  llPutTy(ret);
  putStr(" @");
  putStr(name);
  putCh('(');

  int c = 0;
  while(c < argc) {
    llPutTy(args[c]);
    putStr(" %");
    putNum(c);

    if(c != argc-1) putStr(", ");
    c++;
  }

  putStrLn(") {");
}

void llFuncEnd() {
  putStrLn("}\n");
}

void llBbBegin(char* name) {
  putStr(name);
  putStrLn(":");
}

void llBbEnd() {
  putCh('\n');
}

void llPutVar(int id) {
  putCh('%');
  putNum(id);
}

int llAlign(int ty) {
  if(ty == TY_I1) return 1;
  else if(ty == TY_I8) return 1;
  else if(ty == TY_I32) return 4;
  else if(ty == TY_I64) return 8;
  else if(ty >= TY_I8_REF && ty <= TY_I64_REF_REF) return 8;

  panic("Unknown type.");
  return 0;
}

void llPutAlign(int ty) {
  putStr("align ");
  putNum(llAlign(ty));
}

void llPutAsg(int var) {
  putStr("  ");
  llPutVar(var);
  putStr(" = ");
}

void llInstV(char* name, int var) {
  putStr("  ");
  putStr(name);
  putCh(' ');
  llPutTy(lVarType(var));
  putCh(' ');
  llPutVar(var);
  putCh('\n');
}

void llInstNVAsg(char* name, int dst, int src1, int src2) {
  int ty = lVarType(dst);
  if(ty != lVarType(src2)) panic("Type unmatched");

  llPutAsg(dst);
  putStr(name);
  putCh(' ');
  llPutTy(ty);
  putCh(' ');
  putNum(src1);
  putStr(", ");
  llPutVar(src2);
  putCh('\n');
}

void llIcmpNVAsg(char* cond, int dst, int src1, int src2) {
  int ty = lVarType(src2);
  if(lVarType(dst) != TY_I1) panic("Type unmatched");

  llPutAsg(dst);
  putStr("icmp ");
  putStr(cond);
  putCh(' ');
  llPutTy(ty);
  putCh(' ');
  putNum(src1);
  putStr(", ");
  llPutVar(src2);
  putCh('\n');
}

void llInstVVAsg(char* name, int dst, int src1, int src2) {
  int ty = lVarType(dst);
  if(ty != lVarType(src1)) panic("Type unmatched");
  if(ty != lVarType(src2)) panic("Type unmatched");

  llPutAsg(dst);
  putStr(name);
  putCh(' ');
  llPutTy(ty);
  putCh(' ');
  llPutVar(src1);
  putStr(", ");
  llPutVar(src2);
  putCh('\n');
}

void llIcmpVVAsg(char* cond, int dst, int src1, int src2) {
  int ty = lVarType(src1);
  if(lVarType(dst) != TY_I1) panic("Type unmatched");
  if(lVarType(src2) != ty) panic("Type unmatched");

  llPutAsg(dst);
  putStr("icmp ");
  putStr(cond);
  putCh(' ');
  llPutTy(ty);
  putCh(' ');
  llPutVar(src1);
  putStr(", ");
  llPutVar(src2);
  putCh('\n');
}

void llZeroExt(int dst, int src) {
  llPutAsg(dst);
  putStr("zext ");
  llPutTy(lVarType(src));
  putCh(' ');
  llPutVar(src);
  putStr(" to ");
  llPutTy(lVarType(dst));
  putCh('\n');
}

void llAlloca(int var) {
  int ty = llDerefTy(lVarType(var));
  llPutAsg(var);
  putStr("alloca ");
  llPutTy(ty);
  putStr(", ");
  llPutAlign(ty);
  putCh('\n');
}

void llStoreVal(int var, int val) {
  int ty = llDerefTy(lVarType(var));

  putStr("  store ");
  llPutTy(ty);
  putCh(' ');
  putNum(val);
  putStr(", ");
  llPutTy(llRefTy(ty));
  putCh(' ');
  llPutVar(var);
  putStr(", ");
  llPutAlign(ty);
  putCh('\n');
}

void llStore(int dst, int src) {
  int ty = lVarType(src);
  if(llRefTy(ty) != lVarType(dst)) panic("Type unmatched.");

  putStr("  store ");
  llPutTy(ty);
  putCh(' ');
  llPutVar(src);
  putStr(", ");
  llPutTy(llRefTy(ty));
  putCh(' ');
  llPutVar(dst);
  putStr(", ");
  llPutAlign(ty);
  putCh('\n');
}

void llLoad(int dst, int src) {
  int ty = lVarType(dst);
  if(llRefTy(ty) != lVarType(src)) panic("Type unmatched.");

  llPutAsg(dst);
  putStr("load ");
  llPutTy(ty);
  putStr(", ");
  llPutTy(llRefTy(ty));
  putCh(' ');
  llPutVar(src);
  putStr(", ");
  llPutAlign(ty);
  putCh('\n');
}
