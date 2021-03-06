#include"gen.h"


void llPutTy(int ty) {
  char* types[22] = {
    "unknown", "void",
    "i1",     "i8",     "i32",     "i64",
    "i1*",    "i8*",    "i32*",    "i64*",
    "i1**",   "i8**",   "i32**",   "i64**",
    "i1***",  "i8***",  "i32***",  "i64***",
    "i1****", "i8****", "i32****", "i64****"
  };

  if(ty == TY_UNKNOWN) {
    panic("Unknown type.");
  }

    putStr(types[ty]);
}

int llIsRef(int ty) {
  return ty >= TY_I1_REF && ty <= TY_I64_REF_REF_REF_REF;
}

int llIsUnsigned(int ty) {
  return ty == TY_I1 || ty == TY_U8 || llIsRef(ty);
}

int llRefTy(int ty) {
  if(ty <= TY_VOID) panic("Unknown type.");
  if(ty >= TY_I1_REF_REF_REF_REF) panic("Can't refer the 4th reference.");

  return ty + 4;
}

int llDerefTy(int ty) {
  if(ty <= TY_VOID) panic("Unknwon type.");
  if(ty <= TY_I64) panic("Can't derefer the entity type.");

  return ty - 4;
}

void llPutBb(int id, int bb) {
  char* bbs[7] = {
    "unknown",
    "if.begin", "if.else", "if.end",
    "while.eval", "while.begin", "while.end"
  };

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
  else if(ty == TY_I64) return 8;
  else if(ty >= TY_I1_REF && ty <= TY_I64_REF_REF_REF_REF) return 8;

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

void llDefGVar(char* buf, int len, int ty) {
  putCh('@');
  write(buf, len);
  putStr(" = global ");
  llPutTy(ty);
  putCh(' ');
  if(llIsRef(ty)) {
    putStr("null");
  } else {
    putCh('0');
  }
  putStr(", ");
  llPutAlign(ty);
  putStrLn("\n");
}

void llDefGArray(char* buf, int len, int ty, int c) {
  putCh('@');
  write(buf, len);
  putStr(" = global [");
  putNum(c);
  putStr(" x ");
  llPutTy(ty);
  putStr("] zeroinitializer, ");
  llPutAlign(ty);
  putStrLn("\n");
}

void llRetV(int var) {
  int ty = lVarType(var);
  putStr("  ret ");
  llPutTy(ty);
  if(ty != TY_VOID) {
    putCh(' ');
    llPutVar(var);
  }
  putCh('\n');
}

void llRet(int ty) {
  putStr("  ret ");
  llPutTy(ty);
  if(llIsRef(ty)) {
    putStr(" null");
  } else if(ty != TY_VOID){
    putStr(" 0");
  }

  putCh('\n');
}
void llRetN(int ty, int val) {
  putStr("  ret ");
  llPutTy(ty);
  if(ty != TY_VOID) {
    putCh(' ');
    putNum(val);
  }
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

void llConv(int dst, int src) {
  int dst_ty = lVarType(dst);
  int src_ty = lVarType(src);
  if (dst_ty <= TY_VOID) panic("Can't convert to void.");
  if (src_ty <= TY_VOID) panic("Can't convert from void.");
  if (dst_ty == src_ty) return;

  llPutAsg(dst);

  if(llIsRef(dst_ty)) {
    if(llIsRef(src_ty)) {
      putStr("bitcast");
    } else {
      putStr("inttoptr");
    }
  } else if(llIsRef(src_ty)) {
    putStr("ptrtoint");
  } else if(dst_ty < src_ty) {
    putStr("trunc");
  } else if(llIsUnsigned(src_ty)) {
    putStr("zext");
  } else {
    putStr("sext");
  }

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

void llAllocaArray(int var, int ty, int c) {
  llPutAsg(var);
  putStr("alloca [");
  putNum(c);
  putStr(" x ");
  llPutTy(ty);
  putStr("], ");
  llPutAlign(ty);
  putCh('\n');
}

void llArrayElem(int dst, int src, int id) {
  llPutAsg(dst);
  putStr("getelementptr ");
  llPutTy(llDerefTy(lVarType(dst)));
  putStr(", ");
  llPutTy(lVarType(src));
  putCh(' ');
  llPutVar(src);
  putStr(", ");
  llPutTy(lVarType(id));
  putCh(' ');
  llPutVar(id);
  putCh('\n');
}

void llArrayToPtr(int dst, int src, int c) {
  int ty = llDerefTy(lVarType(dst));
  llPutAsg(dst);
  putStr("bitcast [");
  putNum(c);
  putStr(" x ");
  llPutTy(ty);
  putStr("]* ");
  llPutVar(src);
  putStr(" to ");
  llPutTy(llRefTy(ty));
  putCh('\n');
}

void llGArrayToPtr(int dst, char* buf, int len, int c) {
  int ty = llDerefTy(lVarType(dst));
  llPutAsg(dst);
  putStr("bitcast [");
  putNum(c);
  putStr(" x ");
  llPutTy(ty);
  putStr("]* @");
  write(buf, len);
  putStr(" to ");
  llPutTy(llRefTy(ty));
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

void llStoreG(int var, char* name, int len) {
  int ty = llDerefTy(lVarType(var));

  putStr("  store ");
  llPutTy(ty);
  putStr(" @");
  write(name, len);
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

void llFuncCall(char* buf, int len, int ret, int* args, int argc) {
  putStr("  call ");
  llPutTy(ret);
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

void llFuncCallAsg(char* buf, int len, int dst, int* args, int argc) {
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
