#include"teal.h"

int group_suf[2] = {OP_INC, OP_DEC};
int count_suf = 2;

int group_unary[3] = {OP_ADD, OP_SUB, OP_NOT};
int count_unary = 3;

int group_add[2] = {OP_ADD, OP_SUB};
int count_add = 2;

int group_mul[3] = {OP_MUL, OP_DIV, OP_REM};
int count_mul = 3;

int group_cmp[4] = {OP_LE, OP_LT, OP_GE, OP_GT};
int count_cmp = 4;

int group_eq[2] = {OP_EQ, OP_NE};
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
      return -1;
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
  if(res < 0) {
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
    return -1;
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
  int var1 = exprAdd();
  if(var1 < 0) {
    return -1;
  }

  int o;
  while((o = theseOp(group_cmp, count_cmp))) {
    int var2 = expExprAdd();

    if(o == OP_LT) {
      var1 = lt(var1, var2);
    } else if(o == OP_LE) {
      var1 = le(var1, var2);
    } else if(o == OP_GT) {
      var1 = gt(var1, var2);
    } else if(o == OP_GE) {
      var1 = ge(var1, var2);
    }
  }

  return var1;
}

int expExprCmp() {
  int res = exprCmp();
  if(res < 0) {
    panicParse("exprCmp");
  }
  return res;
}

int exprEq() {
  int var1 = exprCmp();
  if(var1 < 0) {
    return -1;
  }

  int o;
  while((o = theseOp(group_eq, count_eq))) {
    int var2 = expExprCmp();

    if(o == OP_EQ) {
      var1 = eq(var1, var2);
    } else if(o == OP_NE) {
      var1 = ne(var1, var2);
    }
  }

  return var1;
}

int expExprEq() {
  int res = exprEq();
  if(res < 0) {
    panicParse("exprEq");
  }
  return res;
}

int exprAnd() {
  int var1 = exprEq();
  if(var1 < 0) {
    return -1;
  }

  int o;
  while((o = theseOp(group_and, count_and))) {
    int var2 = expExprEq();

    if(o == OP_AND) var1 = and_(var1, var2);
  }

  return var1;
}

int expExprAnd() {
  int res = exprAnd();
  if(res < 0) {
    panicParse("exprAnd");
  }
  return res;
}

int exprOr() {
  int var1 = exprAnd();
  if(var1 < 0) {
    return -1;
  }

  int o;
  while((o = theseOp(group_or, count_or))) {
    int var2 = expExprAnd();

    if(o == OP_OR) var1 = or_(var1, var2);
  }

  return var1;
}

int expExprOr() {
  int res = exprOr();
  if(res < 0) {
    panicParse("exprOr");
  }
  return res;
}

int expExprAsg();

int exprAsg() {
  int m = mark();
  int var = lVal();
  if(var < 0) {
    unmark(m);
    return exprOr();
  }

  int o = theseOp(group_asg, count_asg);
  if(o) {
    unmark(m);
    int src = expExprAsg();

    if(o == OP_ADDASG) src = add(var, src);
    else if(o == OP_SUBASG) {
      src = sub(var, src);
    } else if(o == OP_MULASG) {
      src = mul(var, src);
    } else if(o == OP_DIVASG) {
      src = div(var, src);
    } else if(o == OP_REMASG) {
      src = rem(var, src);
    } else if(o != OP_ASG) {
      panic("Unknown operator");
    }

    var = asg(var, src);
  } else {
    jump(m);
    return exprOr();
  }

  return var;
}

int expExprAsg() {
  int res = exprAsg();
  if(res < 0) {
    panicParse("exprAsg");
  }
  return res;
}

int expr() {
  return exprAsg();
}

int expExpr() {
  int res = expr();
  if(res < 0) {
    panicParse("expr");
  }
  return res;
}

int stmt();
void expStmt();

int stmtSingle() {
  if(expr() < 0) return 0;

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
  int var = expExpr();
  expThisCh(')');

  int id = ifBegin(var);
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
  int var = expExpr();
  expThisCh(')');
  whileEval(id, var);

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
  program();
  funcEnd();

  if(!atEof()) {
    ePutStrLn("WARN: Unread characters are remain.");
  }
}
