#include"gen.h"

int unaryOp(int op, int var) {
  if(op == OP_ADD) return var;

  if(op == OP_DEREF) return derefVar(var);
  if(op == OP_REF) return refVar(var);

  int ty = llDerefTy(lVarType(var));
  int dvar;
  int tmp_var;

  if(op == OP_INC || op == OP_DEC) {
    if(llIsRef(ty)) {
      dvar = castVar(var, TY_I64);
      dvar = derefVar(dvar);
      tmp_var = lTmpVar(TY_I64);
    } else {
      dvar = derefVar(var);
      tmp_var = lTmpVar(ty);
    }
    if(op == OP_INC) llInstNVAsg("add", tmp_var, 1, dvar);
    else llInstNVAsg("add", tmp_var, -1, dvar);
  } else if(op == OP_SUB) {
    if(llIsUnsigned(ty)) {
      panic("Can't negate an unsigned value.");
      return -2;
    }
    dvar = derefVar(var);
    tmp_var = lTmpVar(ty);
    llInstNVAsg("sub", tmp_var, 0, dvar);
  } else if(op == OP_NOT) {
    dvar = derefVar(var);
    tmp_var = lTmpVar(TY_I1);
    llIcmpNVAsg("eq", tmp_var, 0, dvar);
  } else {
    panic("Invalid Unary operator.");
    return -2;
  }

  int new_var = castVar(refVar(tmp_var), ty);

  if(op == OP_INC || op == OP_DEC) {
    llStore(var, derefVar(new_var));
    return var;
  }

  return new_var;
}

int arithOp(int op, int var1, int var2) {
  int ty1 = llDerefTy(lVarType(var1));
  int ty2 = llDerefTy(lVarType(var2));

  int dvar1;
  int dvar2;
  int tmp_var;
  int new_var;

  if(op == OP_ADD) {
    if(llIsRef(ty1)) {
      if(llIsRef(ty2)) {
        panic("Can't add pointer and pointer.");
        return -2;
      }
      var1 = castVar(var1, TY_I64);
      var2 = castVar(var2, TY_I64);
      dvar1 = derefVar(var1);
      dvar2 = derefVar(var2);
      tmp_var = lTmpVar(TY_I64);
      llInstVVAsg("add", tmp_var, dvar1, dvar2);
      return castVar(refVar(tmp_var), ty1);
    }

    dvar1 = derefVar(var1);
    var2 = castVar(var2, ty1);
    dvar2 = derefVar(var2);
    new_var = lTmpVar(ty1);
    llInstVVAsg("add", new_var, dvar1, dvar2);
    return refVar(new_var);
  } else if(op == OP_SUB) {
    if(llIsRef(ty1)) {
      var1 = castVar(var1, TY_I64);
      var2 = castVar(var2, TY_I64);
      dvar1 = derefVar(var1);
      dvar2 = derefVar(var2);
      tmp_var = lTmpVar(TY_I64);
      llInstVVAsg("sub", tmp_var, dvar1, dvar2);
      if(llIsRef(ty2)) {
        return refVar(tmp_var);
      } else {
        return castVar(refVar(tmp_var), ty1);
      }
    }

    dvar1 = derefVar(var1);
    var2 = castVar(var2, ty1);
    dvar2 = derefVar(var2);
    new_var = lTmpVar(ty1);
    llInstVVAsg("sub", new_var, dvar1, dvar2);
    return refVar(new_var);
  }

  if(llIsRef(ty1) || llIsRef(ty2)) {
    panic("Can't multiple or divide pointer.");
    return -2;
  }

  dvar1 = derefVar(var1);
  var2 = castVar(var2, ty1);
  dvar2 = derefVar(var2);
  new_var = lTmpVar(ty1);

  if(op == OP_MUL) llInstVVAsg("mul", new_var, dvar1, dvar2);
  else if(op == OP_DIV){
    if(llIsUnsigned(ty1)) llInstVVAsg("udiv", new_var, dvar1, dvar2);
    else llInstVVAsg("sdiv", new_var, dvar1, dvar2);
  } else if(op == OP_REM) {
    if(llIsUnsigned(ty1)) llInstVVAsg("urem", new_var, dvar1, dvar2);
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
  llConv(new_var, tmp_var);
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

int asgOp(int op, int var1, int var2) {
  if(op != OP_ASG) var2 = arithOp(op - 17, var1, var2);

  var2 = castVar(var2, llDerefTy(lVarType(var1)));
  int dvar2 = derefVar(var2);
  llStore(var1, dvar2);
  return var1;
}

int binOp(int op, int var1, int var2) {
  int ty = llDerefTy(lVarType(var1));
  if(op >= OP_LT && op <= OP_OR) {
    if(llIsRef(ty)) {
      ty = TY_I64;
      var1 = castVar(var1, TY_I64);
      var2 = castVar(var2, TY_I64);
    } else {
      var2 = castVar(var2, ty);
    }
  }

  int new_var;

  if(op >= OP_ADD && op <= OP_REM) new_var = arithOp(op, var1, var2);
  else if(op >= OP_LT && op <= OP_NE) new_var = cmpOp(op, ty, var1, var2);
  else if(op == OP_AND || op == OP_OR) new_var = andOr(op, ty, var1, var2);
  else if(op >= OP_ASG || op <= OP_REMASG) new_var = asgOp(op, var1, var2);
  else {
    panic("Invalid binary operator.");
    return -2;
  }

  if(ty == TY_I64) new_var = castVar(new_var, TY_I32);

  return new_var;
}

int arrayElem(int var, int id) {
  var = derefVar(var);
  int ty = lVarType(var);
  id = castVar(id, TY_I64);
  id = derefVar(id);
  int res = lTmpVar(ty);
  llArrayElem(res, var, id);
  return res;
}

void ret(int var) {
  if(var==VAR_VOID) llRetV(var);
  else {
    var = castVar(var, retTy());
    llRetV(derefVar(var));
  }
  // Skip the implicit basic block.
  lVarAdd(NULL, 0, TY_UNKNOWN);
}
