#include"gen.h"
#define MAX_VARS 1000

int lvars_offset;
char* lvars[MAX_VARS];
int lvars_len[MAX_VARS];
int lvars_ty[MAX_VARS];

void cleanVar() {
  lvars_offset = 0;

  int c = 0;
  while(c < MAX_VARS) {
    lvars[c] = NULL;
    lvars_len[c] = 0;
    lvars_ty[c] = TY_UNKNOWN;
    c++;
  }
}

int lVarFind(char* cur, int len) {
  int c = 0;
  while(!(strCmp(lvars[c], cur, len) && len == lvars_len[c]) && c < lvars_offset) c++;

  if(c == lvars_offset) return -1;

  return c;
}

int lVarAdd(char* buf, int len, int ty) {
  if(lvars_offset == MAX_VARS) panic("Too many variables.");

  int id = lvars_offset;

  lvars_offset++;
  lvars[id] = buf;
  lvars_len[id] = len;
  lvars_ty[id] = ty;

  return id;
}

void addArgs(char** args, int* args_len, int* args_ty, int argc) {
  lvars_offset = argc + 1;
  int c = 0;
  while(c < argc) {
    lvars_ty[c] = args_ty[c];
    int id = lVarAdd(args[c], args_len[c], llRefTy(args_ty[c]));
    llAlloca(id);
    llStore(id, c);
    c++;
  }
}

int lTmpVar(int ty) {
  return lVarAdd(NULL, 0, ty);
}

int lVarType(int id) {
  if(id == VAR_VOID) return TY_VOID;
  return lvars_ty[id];
}

int constNum(int ty, int val) {
  int var = lTmpVar(llRefTy(ty));
  llAlloca(var);
  llStoreVal(var, val);

  return var;
}

int refVar(int var) {
  int res = lTmpVar(llRefTy(lVarType(var)));
  llAlloca(res);
  llStore(res, var);
  return res;
}

int derefVar(int var) {
  int res = lTmpVar(llDerefTy(lVarType(var)));
  llLoad(res, var);
  return res;
}

int defVar(char* buf, int len, int ty) {
  int var = lVarAdd(buf, len, llRefTy(ty));
  llAlloca(var);

  return var;
}

int defArray(char* buf, int len, int ty, int c) {
  int tmp_var1 = lTmpVar(TY_UNKNOWN);
  llAllocaArray(tmp_var1, ty, c);
  int tmp_var2 = lTmpVar(llRefTy(ty));
  llArrayToPtr(tmp_var2, tmp_var1, c);
  int var = defVar(buf, len, llRefTy(ty));
  llStore(var, tmp_var2);
  return var;
}

int tmpStr(char* str) {
  int len = length(str);
  int var = defArray(NULL, 0, TY_U8, len+1);
  int c = 0;
  while(c <= len) {
    int ptr = derefVar(var);
    int new_ptr = lTmpVar(TY_I64);
    llConv(new_ptr, ptr);
    int tmp_var = lTmpVar(TY_I64);
    llInstNVAsg("add", tmp_var, c, new_ptr);
    int new_tmp = lTmpVar(TY_U8_REF);
    llConv(new_tmp, tmp_var);
    llStoreVal(new_tmp, str[c]);
    c++;
  }

  return var;
}

int castVar(int var, int ty) {
  if(llDerefTy(lVarType(var)) == ty) return var;

  int dvar = derefVar(var);
  int new_var = lTmpVar(ty);
  llConv(new_var, dvar);
  return refVar(new_var);
}
