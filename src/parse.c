#include"teal.h"

int group_suf[2] = {OP_INC, OP_DEC};
int count_suf = 2;

int group_unary[3] = {OP_ADD, OP_SUB, OP_NOT};
int count_unary = 3;

int group_add[2] = {OP_ADD, OP_SUB};
int count_add = 2;

int group_mul[3] = {OP_MUL, OP_DIV, OP_REM};
int count_mul = 3;

int group_cmp[4] = {OP_LESS, OP_LEQ, OP_MEQ, OP_MORE};
int count_cmp = 4;

int group_eq[2] = {OP_EQ, OP_NEQ};
int count_eq = 2;

int group_and[1] = {OP_AND};
int count_and = 1;

int group_or[1] = {OP_OR};
int count_or = 1;

int group_asg[6] = {OP_ASG, OP_ADDASG, OP_SUBASG, OP_MULASG, OP_DIVASG, OP_REMASG};
int count_asg = 6;

void panicParse(char* at) {
  ePutStr("Parse failed (at \"");
  ePutStr(at);
  ePutStrLn("\")");
  sysExit(1);
}

int lVal() {
  return lexIdent();
}

int expLVal() {
  int l = lVal();
  if(l < 0) {
    panicParse("lVal");
  }

  return l;
}

int expExpr();

int exprFactor() {
  if(thisCh('(')) {
    int var = expExpr();
    expThisCh(')');
    return var;
  } else {
    int i = lexNum();
    if(i != -1) {
      return constNum(TY_I32, i);
    } else {
      return lVal();
    }
  }
}

int expExprFactor() {
  int res = exprFactor();
  if(res < 0) {
    panicParse("exprFactor");
  }
  return res;
}

int exprSuf() {
  int m = mark();
  int var = lVal();
  if(var < 0) {
    unmark(m);
    return exprFactor();
  }

  int o = theseOp(group_suf, count_suf);
  if(o) {
    unmark(m);
  } else {
    jump(m);
    return exprFactor();
  }

  while(o) {
    if (o == OP_INC) {
      var = inc(var);
    } else if (o == OP_DEC) {
      var = dec(var);
    }

    o = theseOp(group_suf, count_suf);
  }

  return var;
}

int expExprSuf() {
  int res = exprSuf();
  if(res < 0) {
    panicParse("exprSuf");
  }
  return res;
}

int exprUnary() {
  int o = theseOp(group_unary, count_unary);
  int var = exprSuf();
  if(var < 0) {
    if(o) {
      panicParse("exprUnary");
    } else {
      return TY_UNKNOWN;
    }
  }

  if(o == OP_SUB) {
    var = neg(var);
  } else if(o == OP_NOT) {
    var = not_(var);
  }

  return var;
}

int expExprUnary() {
  int res = exprUnary();
  if(!res) {
    panicParse("exprUnary");
  }
  return res;
}

int exprMul() {
  int var1 = exprUnary();
  if(var1 < 0){
    return -1;
  }

  int o;
  while((o = theseOp(group_mul, count_mul))) {
    int var2 = expExprUnary();

    if(o == OP_MUL) {
      var1 = mul(var1, var2);
    } else if (o == OP_DIV) {
      var1 = div(var1, var2);
    } else if (o == OP_REM) {
      var1 = rem(var1, var2);
    } else {
      panic("Unknown operator");
    }

  }

  return var1;
}

int expExprMul() {
  int res = exprMul();
  if(res < 0) {
    panicParse("exprMul");
  }
  return res;
}

int exprAdd() {
  int var1 = exprMul();
  if(var1 < 0) {
    return TY_UNKNOWN;
  }

  int o;
  while((o = theseOp(group_add, count_add))) {
    int var2 = expExprMul();

    if(o == OP_ADD) {
      var1 = add(var1, var2);
    } else if (o == OP_SUB) {
      var1 = sub(var1, var2);
    } else {
      panic("Unknown operator");
    }
  }

  return var1;
}

int expExprAdd() {
  int res = exprAdd();
  if(res < 0) {
    panicParse("exprAdd");
  }
  return res;
}

int exprCmp() {
  int ty1 = exprAdd();
  if(!ty1) {
    return TY_UNKNOWN;
  }

  int o;
  while((o = theseOp(group_cmp, count_cmp))) {
    int ty2 = expExprAdd();
    if(ty1 != ty2) {
      panic("Type unmatched");
    }

    if(o == OP_LESS) {
      less(ty1);
    } else if(o == OP_LEQ) {
      leq(ty1);
    } else if(o == OP_MORE) {
      more(ty1);
    } else if(o == OP_MEQ) {
      meq(ty1);
    }
  }

  return ty1;
}

int expExprCmp() {
  int res = exprCmp();
  if(!res) {
    panicParse("exprCmp");
  }
  return res;
}

int exprEq() {
  int ty1 = exprCmp();
  if(!ty1) {
    return TY_UNKNOWN;
  }

  int o;
  while((o = theseOp(group_eq, count_eq))) {
    int ty2 = expExprCmp();
    if(ty1 != ty2) {
      panic("Type unmatched");
    }

    if(o == OP_EQ) {
      eq(ty1);
    } else if(o == OP_NEQ) {
      neq(ty1);
    }
  }

  return ty1;
}

int expExprEq() {
  int res = exprEq();
  if(!res) {
    panicParse("exprEq");
  }
  return res;
}

int exprAnd() {
  int ty1 = exprEq();
  if(!ty1) {
    return TY_UNKNOWN;
  }

  int o;
  while((o = theseOp(group_and, count_and))) {
    int ty2 = expExprEq();
    if(ty1 != ty2) {
      panic("Type unmatched");
    }

    if(o == OP_AND) and_();
  }

  return ty1;
}

int expExprAnd() {
  int res = exprAnd();
  if(!res) {
    panicParse("exprAnd");
  }
  return res;
}

int exprOr() {
  int ty1 = exprAnd();
  if(!ty1) {
    return TY_UNKNOWN;
  }

  int o;
  while((o = theseOp(group_or, count_or))) {
    int ty2 = expExprAnd();
    if(ty1 != ty2) {
      panic("Type unmatched");
    }

    if(o == OP_OR) or_();
  }

  return ty1;
}

int expExprOr() {
  int res = exprOr();
  if(!res) {
    panicParse("exprOr");
  }
  return res;
}

int expExprAsg();

int exprAsg() {
  int m = mark();
  int l = lVal();
  int ty = lVarType(l);
  if(!l) {
    unmark(m);
    return exprOr();
  }

  int o = theseOp(group_asg, count_asg);
  if(o) {
    unmark(m);
    if(ty != expExprAsg()) {
      panic("Type unmatched");
    }

    if(o == OP_ASG) {
      asg(l);
    } else if(o == OP_ADDASG) {
      addasg(l);
    } else if(o == OP_SUBASG) {
      subasg(l);
    } else if(o == OP_MULASG) {
      mulasg(l);
    } else if(o == OP_DIVASG) {
      divasg(l);
    } else if(o == OP_REMASG) {
      remasg(l);
    }
  } else {
    jump(m);
    return exprOr();
  }

  return ty;
}

int expExprAsg() {
  int res = exprAsg();
  if(!res) {
    panicParse("exprAsg");
  }
  return res;
}

int expr() {
  return exprAsg();
}

int expExpr() {
  int res = expr();
  if(!res) {
    panicParse("expr");
  }
  return res;
}

int stmt();
void expStmt();

int stmtSingle() {
  if(!expr()) return 0;

  expThisCh(';');
  return 1;
}

int stmtMulti() {
  if(!thisCh('{')) return 0;

  while(stmt()) {}
  expThisCh('}');
  return 1;
}

int stmtRet() {
  if(!thisStr("return")) return 0;

  int var = expExpr();
  expThisCh(';');
  ret(var);
  return 1;
}

int stmtIf() {
  if(!thisStr("if")) return 0;

  expThisCh('(');
  expExpr();
  expThisCh(')');

  int id = ifBegin();
  expStmt();

  ifElse(id);
  if(thisStr("else")) expStmt();

  ifEnd(id);

  return 1;
}

int stmtWhile() {
  if(!thisStr("while")) return 0;

  int id = whileBegin();

  expThisCh('(');
  expExpr();
  expThisCh(')');
  whileEval(id);

  expStmt();
  whileEnd(id);

  return 1;
}

int stmt() {
  if(!stmtMulti() && !stmtRet() && !stmtIf() && !stmtWhile()) {
    return stmtSingle();
  }

  return 1;
}


void expStmt() {
  if(!stmt()) {
    panicParse("stmt");
  }
}

void program() {
  stmt();
}

void parse() {
  funcBegin("main", TY_I32, NULL, 0);
  llBbBegin("entry");
  program();
  funcEnd();

  if(!atEof()) {
    ePutStrLn("WARN: Unread characters are remain.");
  }
}
