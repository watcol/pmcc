#include"gen.h"

char* types[14] = {
  "unknown", "void",
  "i1",   "u8",   "i32",
  "i1*",  "u8*",  "i32*",
  "i1**", "u8**", "i32**",
  "i1***", "u8***", "i32***"
};

char* bbs[7] = {
  "unknown",
  "if.begin", "if.else", "if.end",
  "while.eval", "while.begin", "while.end"
};

void llPutTy(int ty) {
  if(ty == TY_UNKNOWN) {
    panic("Unknown type.");
  }

    putStr(types[ty]);
}

int llIsRef(int ty) {
  return ty >= TY_I1_REF && ty <= TY_I32_REF_REF_REF;
}

int llIsUnsigned(int ty) {
  return ty == TY_U8 || llIsRef(ty);
}

int llRefTy(int ty) {
  if(ty <= TY_VOID) panic("Unknown type.");
  if(ty >= TY_I1_REF_REF_REF) panic("Can't refer the reference of reference of reference.");

  return ty + 3;
}

int llDerefTy(int ty) {
  if(ty <= TY_VOID) panic("Unknwon type.");
  if(ty <= TY_I32) panic("Can't derefer the entity type.");

  return ty - 3;
}

void llPutBb(int id, int bb) {
  if(bb <= BB_UNKNOWN || bb > BB_WHILE_END) panic("Unknown BasicBlock.");
  putStr(bbs[bb]);
  putCh('-');
  putNum(id);
}

void llFuncDecl(char* name, int len, int ret, int* args, int argc) {
  putStr("declare ");
  llPutTy(ret);
  putStr(" @");
  write(name, len);
  putCh('(');

  int c = 0;
  while(c < argc) {
    llPutTy(args[c]);

    if(c != argc-1) putStr(", ");
    c++;
  }

  putStrLn(")\n");
}

void llFuncBegin(char* name, int len, int ret, int* args, int argc) {
  putStr("define ");
  llPutTy(ret);
  putStr(" @");
  write(name, len);
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


void llBb(int id, int bb) {
  putCh('\n');
  llPutBb(id, bb);
  putStrLn(":");
}

void llPutVar(int id) {
  putCh('%');
  putNum(id);
}

int llAlign(int ty) {
  if(ty == TY_I1) return 1;
  else if(ty == TY_U8) return 1;
  else if(ty == TY_I32) return 4;
  else if(ty >= TY_I1_REF && ty <= TY_I32_REF_REF_REF) return 8;

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

void llInstN(char* name, int ty, int val) {
  putStr("  ");
  putStr(name);
  putCh(' ');
  llPutTy(ty);
  putCh(' ');
  putNum(val);
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

void llConv(char* name, int dst, int src) {
  llPutAsg(dst);
  putStr(name);
  putCh(' ');
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

void llFuncCall(char* buf, int len, int dst, int* args, int argc) {
  llPutAsg(dst);
  putStr("call ");
  llPutTy(lVarType(dst));
  putStr(" @");
  write(buf, len);
  putCh('(');
  int c = 0;
  while(c < argc) {
    llPutTy(lVarType(args[c]));
    putCh(' ');
    llPutVar(args[c]);
    c++;
    if(c != argc) putStr(", ");
  }
  putStrLn(")");
}

void llBr(int id, int bb) {
  putStr("  br label %");
  llPutBb(id, bb);
  putCh('\n');
}

void llBrCond(int cond, int id, int bb1, int bb2) {
  if(lVarType(cond) != TY_I1) panic("Type unmatched.");

  putStr("  br i1 ");
  llPutVar(cond);
  putStr(", label %");
  llPutBb(id, bb1);
  putStr(", label %");
  llPutBb(id, bb2);
  putCh('\n');
}
