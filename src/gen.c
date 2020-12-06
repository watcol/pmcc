#include"teal.h"
#define MAX_VARS 100

int lvars_offset;
char* lvars[MAX_VARS];
int lvars_ty[MAX_VARS];

void cleanVar() {
  lvars_offset = 0;

  int c = 0;
  while(c < MAX_VARS) {
    lvars[c] = NULL;
    lvars_ty[c] = TY_UNKNOWN;
    c++;
  }
}

void initCode() {
  cleanVar();
}

void funcBegin(char* name, int ret, int* args, int argc) {
  llFuncBegin(name, ret, args, argc);

  lvars_offset = argc;
  int c = 0;
  while(c < argc) {
    lvars_ty[c] = args[c];
    c++;
  }
}

void funcEnd() {
  cleanVar();
  llFuncEnd();
}

int lVarFind(char* cur, int len) {
  int c = 0;
  while(!strCmp(lvars[c], cur, len) && c < lvars_offset) c++;

  if(c == lvars_offset) return -1;

  return c;
}

int lVarAdd(char* buf, int ty) {
  if(lvars_offset == MAX_VARS) panic("Too many variables.");

  int id = lvars_offset;

  lvars_offset++;
  lvars[id] = buf;
  lvars_ty[id] = ty;

  return id;
}

int lVarType(int id) {
  return lvars_ty[id];
}

int constNum(int ty, int val) {
  int var = lVarAdd(NULL, llRefTy(ty));
  llAlloca(var);
  llStoreVal(var, val);

  return var;
}

int refVar(int var) {
  int res = lVarAdd(NULL, llRefTy(lVarType(var)));
  llAlloca(res);
  llStore(res, var);
  return res;
}

int derefVar(int var) {
  int res = lVarAdd(NULL, llDerefTy(lVarType(var)));
  llLoad(res, var);
  return res;
}

int defVar(char* buf, int len, int ty) {
  int var = lVarFind(buf, len);
  if(var == -1) {
    var = lVarAdd(buf, llRefTy(ty));
    llAlloca(var);
  }

  if(lVarType(var) != llRefTy(ty)) panic("Type unmatched");

  return var;
}

int inc(int var) {
  int ty = lVarType(var);

  int dvar = derefVar(var);
  int new_var = lVarAdd(NULL, llDerefTy(ty));

  llInstNVAsg("add", new_var, 1, dvar);
  return refVar(new_var);
}

int dec(int var) {
  int ty = lVarType(var);

  int dvar = derefVar(var);
  int new_var = lVarAdd(NULL, llDerefTy(ty));

  llInstNVAsg("add", new_var, -1, dvar);
  return refVar(new_var);
}

int neg(int var) {
  int ty = lVarType(var);

  int dvar = derefVar(var);
  int new_var = lVarAdd(NULL, llDerefTy(ty));

  llInstNVAsg("sub", new_var, 0, dvar);
  return refVar(new_var);
}

int not_(int var) {
  int ty = lVarType(var);

  int dvar = derefVar(var);
  int tmp_var = lVarAdd(NULL, TY_I1);

  llIcmpNVAsg("eq", tmp_var, 0, dvar);
  int new_var = lVarAdd(NULL, llDerefTy(ty));
  llZeroExt(new_var, tmp_var);

  return refVar(new_var);
}

int mul(int var1, int var2) {
  int ty = lVarType(var1);
  if(ty != lVarType(var2)) panic("Type unmatched");

  int dvar1 = derefVar(var1);
  int dvar2 = derefVar(var2);
  int new_var = lVarAdd(NULL, llDerefTy(ty));

  llInstVVAsg("mul", new_var, dvar1, dvar2);

  return refVar(new_var);
}

int div(int var1, int var2) {
  int ty = lVarType(var1);
  if(ty != lVarType(var2)) panic("Type unmatched");

  int dvar1 = derefVar(var1);
  int dvar2 = derefVar(var2);
  int new_var = lVarAdd(NULL, llDerefTy(ty));

  llInstVVAsg("sdiv", new_var, dvar1, dvar2);

  return refVar(new_var);
}

int rem(int var1, int var2) {
  int ty = lVarType(var1);
  if(ty != lVarType(var2)) panic("Type unmatched");

  int dvar1 = derefVar(var1);
  int dvar2 = derefVar(var2);
  int new_var = lVarAdd(NULL, llDerefTy(ty));

  llInstVVAsg("srem", new_var, dvar1, dvar2);

  return refVar(new_var);
}

int add(int var1, int var2) {
  int ty = lVarType(var1);
  if(ty != lVarType(var2)) panic("Type unmatched");

  int dvar1 = derefVar(var1);
  int dvar2 = derefVar(var2);
  int new_var = lVarAdd(NULL, llDerefTy(ty));

  llInstVVAsg("add", new_var, dvar1, dvar2);

  return refVar(new_var);
}

int sub(int var1, int var2) {
  int ty = lVarType(var1);
  if(ty != lVarType(var2)) panic("Type unmatched");

  int dvar1 = derefVar(var1);
  int dvar2 = derefVar(var2);
  int new_var = lVarAdd(NULL, llDerefTy(ty));

  llInstVVAsg("sub", new_var, dvar1, dvar2);

  return refVar(new_var);
}

int lt(int var1, int var2) {
  int ty = lVarType(var1);
  if(ty != lVarType(var2)) panic("Type unmatched");

  int dvar1 = derefVar(var1);
  int dvar2 = derefVar(var2);
  int tmp_var = lVarAdd(NULL, TY_I1);
  llIcmpVVAsg("slt", tmp_var, dvar1, dvar2);

  int new_var = lVarAdd(NULL, llDerefTy(ty));
  llZeroExt(new_var, tmp_var);

  return refVar(new_var);
}

int le(int var1, int var2) {
  int ty = lVarType(var1);
  if(ty != lVarType(var2)) panic("Type unmatched");

  int dvar1 = derefVar(var1);
  int dvar2 = derefVar(var2);
  int tmp_var = lVarAdd(NULL, TY_I1);
  llIcmpVVAsg("sle", tmp_var, dvar1, dvar2);

  int new_var = lVarAdd(NULL, llDerefTy(ty));
  llZeroExt(new_var, tmp_var);

  return refVar(new_var);
}

int gt(int var1, int var2) {
  int ty = lVarType(var1);
  if(ty != lVarType(var2)) panic("Type unmatched");

  int dvar1 = derefVar(var1);
  int dvar2 = derefVar(var2);
  int tmp_var = lVarAdd(NULL, TY_I1);
  llIcmpVVAsg("sgt", tmp_var, dvar1, dvar2);

  int new_var = lVarAdd(NULL, llDerefTy(ty));
  llZeroExt(new_var, tmp_var);

  return refVar(new_var);
}

int ge(int var1, int var2) {
  int ty = lVarType(var1);
  if(ty != lVarType(var2)) panic("Type unmatched");

  int dvar1 = derefVar(var1);
  int dvar2 = derefVar(var2);
  int tmp_var = lVarAdd(NULL, TY_I1);
  llIcmpVVAsg("sge", tmp_var, dvar1, dvar2);

  int new_var = lVarAdd(NULL, llDerefTy(ty));
  llZeroExt(new_var, tmp_var);

  return refVar(new_var);
}

int eq(int var1, int var2) {
  int ty = lVarType(var1);
  if(ty != lVarType(var2)) panic("Type unmatched");

  int dvar1 = derefVar(var1);
  int dvar2 = derefVar(var2);
  int tmp_var = lVarAdd(NULL, TY_I1);
  llIcmpVVAsg("eq", tmp_var, dvar1, dvar2);

  int new_var = lVarAdd(NULL, llDerefTy(ty));
  llZeroExt(new_var, tmp_var);

  return refVar(new_var);
}

int ne(int var1, int var2) {
  int ty = lVarType(var1);
  if(ty != lVarType(var2)) panic("Type unmatched");

  int dvar1 = derefVar(var1);
  int dvar2 = derefVar(var2);
  int tmp_var = lVarAdd(NULL, TY_I1);
  llIcmpVVAsg("ne", tmp_var, dvar1, dvar2);

  int new_var = lVarAdd(NULL, llDerefTy(ty));
  llZeroExt(new_var, tmp_var);

  return refVar(new_var);
}

void and_() {
  panic("Unimplemented");
}

void or_() {
  panic("Unimplemented");
}

void asg(int type) {
  panic("Unimplemented");
}

void addasg(int type) {
  panic("Unimplemented");
}

void subasg(int type) {
  panic("Unimplemented");
}

void mulasg(int type) {
  panic("Unimplemented");
}

void divasg(int type) {
  panic("Unimplemented");
}

void remasg(int type) {
  panic("Unimplemented");
}

void ret(int var) {
  llInstV("ret", derefVar(var));
}

int ifBegin() {
  panic("Unimplemented");
  return 0;
}

void ifElse(int id) {
  panic("Unimplemented");
}

void ifEnd(int id) {
  panic("Unimplemented");
}

int whileBegin() {
  panic("Unimplemented");
  return 0;
}

void whileEval(int id) {
  panic("Unimplemented");
}

void whileEnd(int id) {
  panic("Unimplemented");
}

