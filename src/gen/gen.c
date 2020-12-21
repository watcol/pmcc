#include"gen.h"
#define MAX_VARS 100
#define MAX_FUNCS 80

int lvars_offset;
char* lvars[MAX_VARS];
int lvars_len[MAX_VARS];
int lvars_ty[MAX_VARS];

int funcs_offset;
char* funcs_name[MAX_FUNCS];
int funcs_len[MAX_FUNCS];
int funcs_ret[MAX_FUNCS];

int bbs_offset;

void cleanVar() {
  lvars_offset = 0;

  int c = 0;
  while(c < MAX_VARS) {
    lvars[c] = NULL;
    lvars_len[c] = 0;
    lvars_ty[c] = TY_UNKNOWN;
    c++;
  }

  bbs_offset = 0;
}

void initCode() {
  cleanVar();

  funcs_offset = 0;
  int c = 0;
  while(c < MAX_FUNCS) {
    funcs_name[c] = NULL;
    funcs_len[c] = 0;
    funcs_ret[c] = TY_UNKNOWN;
    c++;
  }
}

int retTy() {
  return funcs_ret[funcs_offset-1];
}

void funcAdd(char* name, int len, int ret) {
  funcs_name[funcs_offset] = name;
  funcs_len[funcs_offset] = len;
  funcs_ret[funcs_offset] = ret;
  funcs_offset++;
}

int funcFind(char* cur, int len) {
  int c = 0;
  while(!(strCmp(funcs_name[c], cur, len) && len == funcs_len[c]) && c < funcs_offset) c++;

  if(c == funcs_offset) {
    panic("Function not found.");
    return -1;
  }

  return c;
}

void funcDecl(char* name, int len, int ret, int* args, int argc) {
  funcAdd(name, len, ret);
  llFuncDecl(name, len, ret, args, argc);
}

void funcBegin(char* name, int len, int ret, char** args, int* arg_lens, int* arg_tys, int argc) {
  funcAdd(name, len, ret);
  llFuncBegin(name, len, ret, arg_tys, argc);

  // Skip the implicit basic block.
  lvars_offset = argc + 1;
  int c = 0;
  while(c < argc) {
    lvars_ty[c] = arg_tys[c];
    int id = lVarAdd(args[c], arg_lens[c], llRefTy(arg_tys[c]));
    llAlloca(id);
    llStore(id, c);
    c++;
  }
}

void funcEnd() {
  // dummy ret
  int ret_ty = retTy();
  if(ret_ty == TY_VOID) putStrLn("  ret void");
  else llInstN("ret", ret_ty, 1);
  cleanVar();
  llFuncEnd();
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

int lTmpVar(int ty) {
  return lVarAdd(NULL, 0, ty);
}

int lVarType(int id) {
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

int unaryOp(int op, int var) {
  if(op == OP_ADD) return var;

  if(op == OP_DEREF) return derefVar(var);
  if(op == OP_REF) return refVar(var);

  int ty = lVarType(var);
  int dvar = derefVar(var);
  int new_var = lTmpVar(llDerefTy(ty));

  if(op == OP_INC) llInstNVAsg("add", new_var, 1, dvar);
  else if(op == OP_DEC) llInstNVAsg("add", new_var, -1, dvar);
  else if(op == OP_SUB) llInstNVAsg("sub", new_var, 0, dvar);
  else if(op == OP_NOT) {
    int tmp_var = lTmpVar(TY_I1);
    llIcmpNVAsg("eq", tmp_var, 0, dvar);
    llConv("zext", new_var, tmp_var);
  } else {
    panic("Invalid Unary operator.");
  }

  if(op == OP_INC || op == OP_DEC) {
    llStore(var, new_var);
    return var;
  }

  return refVar(new_var);
}

int arithOp(int op, int ty, int var1, int var2) {
  int dvar1 = derefVar(var1);
  int dvar2 = derefVar(var2);
  int new_var = lTmpVar(ty);

  if(op == OP_ADD) llInstVVAsg("add", new_var, dvar1, dvar2);
  else if(op == OP_SUB) llInstVVAsg("sub", new_var, dvar1, dvar2);
  else if(op == OP_MUL) llInstVVAsg("mul", new_var, dvar1, dvar2);
  else if(op == OP_DIV){
    if(llIsUnsigned(ty)) llInstVVAsg("udiv", new_var, dvar1, dvar2);
    else llInstVVAsg("sdiv", new_var, dvar1, dvar2);
  } else if(op == OP_REM) {
    if(llIsUnsigned(ty)) llInstVVAsg("urem", new_var, dvar1, dvar2);
    else llInstVVAsg("srem", new_var, dvar1, dvar2);
  }

  return refVar(new_var);
}

int cmpOp(int op, int ty, int var1, int var2) {
  int dvar1 = derefVar(var1);
  int dvar2 = derefVar(var2);
  int tmp_var = lTmpVar(TY_I1);

  if(op == OP_LT) {
    if(llIsUnsigned(ty)) llIcmpVVAsg("ult", tmp_var, dvar1, dvar2);
    else llIcmpVVAsg("slt", tmp_var, dvar1, dvar2);
  } else if(op == OP_LE) {
    if(llIsUnsigned(ty)) llIcmpVVAsg("ule", tmp_var, dvar1, dvar2);
    else llIcmpVVAsg("sle", tmp_var, dvar1, dvar2);
  } else if(op == OP_GT) {
    if(llIsUnsigned(ty)) llIcmpVVAsg("ugt", tmp_var, dvar1, dvar2);
    else llIcmpVVAsg("sgt", tmp_var, dvar1, dvar2);
  } else if(op == OP_GE) {
    if(llIsUnsigned(ty)) llIcmpVVAsg("uge", tmp_var, dvar1, dvar2);
    else llIcmpVVAsg("sge", tmp_var, dvar1, dvar2);
  } else if(op == OP_EQ) llIcmpVVAsg("eq", tmp_var, dvar1, dvar2);
  else if(op == OP_NE) llIcmpVVAsg("ne", tmp_var, dvar1, dvar2);

  int new_var = lTmpVar(ty);
  llConv("zext", new_var, tmp_var);
  return refVar(new_var);
}

int andOr(int op, int ty, int var1, int var2) {
  int dvar1 = derefVar(var1);
  int dvar2 = derefVar(var2);
  int new_var = lTmpVar(llRefTy(ty));
  llAlloca(new_var);

  int id = ifBegin(var1);
  if(op == OP_AND) llStore(new_var, dvar2);
  else llStore(new_var, dvar1);
  ifElse(id);
  if(op == OP_AND) llStore(new_var, dvar1);
  else llStore(new_var, dvar2);
  ifEnd(id);

  return new_var;
}

int asgOp(int op, int ty, int var1, int var2) {
  if(op != OP_ASG) var2 = arithOp(op - 17, ty, var1, var2);

  int dvar2 = derefVar(var2);
  llStore(var1, dvar2);
  return var1;
}

int binOp(int op, int var1, int var2) {
  int ty = lVarType(var1);
  if(ty != lVarType(var2)) panic("type unmatched.");
  ty = llDerefTy(ty);

  int new_var = -1;

  if(op >= OP_ADD && op <= OP_REM) new_var = arithOp(op, ty, var1, var2);
  else if(op >= OP_LT && op <= OP_NE) new_var = cmpOp(op, ty, var1, var2);
  else if(op == OP_AND || op == OP_OR) new_var = andOr(op, ty, var1, var2);
  else if(op >= OP_ASG || op <= OP_REMASG) new_var = asgOp(op, ty, var1, var2);
  else panic("Invalid binary operator.");

  return new_var;
}

int funcCall(char* buf, int len, int* args, int argc) {
  int c = 0;
  while(c < argc) {
    args[c] = derefVar(args[c]);
    c++;
  }
  int dst = -1;
  int ret_ty = funcs_ret[funcFind(buf, len)];
  if(ret_ty == TY_VOID) {
    llFuncCall(buf, len, ret_ty, args, argc);
  } else {
    dst = lTmpVar(ret_ty);
    llFuncCallAsg(buf, len, dst, args, argc);
    dst = refVar(dst);
  }
  return dst;
}

void ret(int var) {
  if(var==VAR_VOID) putStrLn("  ret void");
  else {
    if(llDerefTy(lVarType(var)) != retTy()) panic("Type unmatched.");
    llInstV("ret", derefVar(var));
  }
  // Skip the implicit basic block.
  lvars_offset++;
}

int ifBegin(int var) {
  bbs_offset++;
  int id = bbs_offset;

  int dvar = derefVar(var);
  int cond = lTmpVar(TY_I1);
  llIcmpNVAsg("ne", cond, 0, dvar);
  llBrCond(cond, id, BB_IF_BEGIN, BB_IF_ELSE);
  llBb(id, BB_IF_BEGIN);

  return id;
}

void ifElse(int id) {
  llBr(id, BB_IF_END);
  llBb(id, BB_IF_ELSE);
}

void ifEnd(int id) {
  llBr(id, BB_IF_END);
  llBb(id, BB_IF_END);
}

int whileBegin() {
  bbs_offset++;
  int id = bbs_offset;
  llBr(id, BB_WHILE_EVAL);
  llBb(id, BB_WHILE_EVAL);
  return id;
}

void whileEval(int id, int var) {
  int dvar = derefVar(var);
  int cond = lTmpVar(TY_I1);
  llIcmpNVAsg("ne", cond, 0, dvar);
  llBrCond(cond, id, BB_WHILE_BEGIN, BB_WHILE_END);

  llBb(id, BB_WHILE_BEGIN);
}

void whileEnd(int id) {
  llBr(id, BB_WHILE_EVAL);
  llBb(id, BB_WHILE_END);
}

