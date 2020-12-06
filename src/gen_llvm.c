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

    put(types[ty]);
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
  put("define ");
  llPutTy(ret);
  put(" @");
  put(name);
  putC('(');

  int c = 0;
  while(c < argc) {
    llPutTy(args[c]);
    put(" %");
    putNum(c);

    if(c != argc-1) put(", ");
    c++;
  }

  putS(") {");
}

void llFuncEnd() {
  putS("}\n");
}

void llBbBegin(char* name) {
  put(name);
  putS(":");
}

void llBbEnd() {
  putC('\n');
}

void llPutVar(int id) {
  putC('%');
  putNum(id);
}

int llAlign(int ty) {
  if(ty == TY_I8) return 1;
  else if(ty == TY_I16) return 2;
  else if(ty == TY_I32) return 4;
  else if(ty == TY_I64) return 8;

  panic("Unknown type.");
  return 0;
}

void llPutAlign(int ty) {
  put("align ");
  putNum(llAlign(ty));
}

void llRetN(int ty, int val) {
  put("  ret ");
  llPutTy(ty);
  putC(' ');
  putNum(val);
  putC('\n');
}

void llAlloca(int var) {
  int ty = llDerefTy(lVarType(var));
  put("  ");
  llPutVar(var);
  put(" = alloca ");
  llPutTy(ty);
  put(", ");
  llPutAlign(ty);
  putC('\n');
}
