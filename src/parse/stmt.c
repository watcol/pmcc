#include"parse.h"

int stmtSingle() {
  if(expr() < -1) return 0;

  expThisCh(';');
  return 1;
}

int stmtMulti() {
  if(!thisCh('{')) return 0;

  while(stmt()) {}
  expThisCh('}');
  return 1;
}

int stmtDecl() {
  int ty = type();
  if(!ty) return 0;

  char* buf = getCursor();
  int len = expIdent();
  if(thisCh('[')) {
    int c = expNum();
    if(thisCh('+')) c += expNum();
    expThisCh(']');
    defArray(buf, len, ty, c);
  } else {
    int var = defVar(buf, len, ty);
    if(thisCh('=')) {
      int val = expExpr();
      binOp(OP_ASG, var, val);
    }
  }

  expThisCh(';');
  return 1;
}

int stmtRet() {
  if(!thisStr("return")) return 0;

  int var;
  if(retTy() == TY_VOID) {
    var = VAR_VOID;
  } else {
    var = expExpr();
  }

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
  if(!stmtMulti() && !stmtDecl() && !stmtRet() && !stmtIf() && !stmtWhile()) {
    return stmtSingle();
  }

  return 1;
}


void expStmt() {
  if(!stmt()) {
    panicParse("stmt");
  }
}
