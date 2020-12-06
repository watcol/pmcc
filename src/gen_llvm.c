#include"teal.h"

char* types[13] = {
  "unknown",
  "i8",   "i16",   "i32",   "i64",
  "i8*",  "i16*",  "i32*",  "i64*",
  "i8**", "i16**", "i32**", "i64**"
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
  if(ty == TY_I8) return 1;
  else if(ty == TY_I16) return 2;
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

void llRetN(int ty, int val) {
  putStr("  ret ");
  llPutTy(ty);
  putCh(' ');
  putNum(val);
  putCh('\n');
}

void llAlloca(int var) {
  int ty = llDerefTy(lVarType(var));
  putStr("  ");
  llPutVar(var);
  putStr(" = alloca ");
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
  llPutVar(var);
  putStr(", ");
  llPutAlign(ty);
}

void llLoad(int dst, int src) {
  int ty = lVarType(dst);
  if(llRefTy(ty) != lVarType(src)) panic("Type unmatched.");

  putStr("  ");
  llPutVar(dst);
  putStr(" = load ");
  llPutTy(ty);
  putStr(", ");
  llPutTy(llRefTy(ty));
  putCh(' ');
  llPutVar(src);
  putStr(", ");
  llPutAlign(ty);
}
