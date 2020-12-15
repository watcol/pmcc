#include"teal.h"

#define MAX_ARGS 10

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

int expr();
int expExpr();

int exprFactor() {
  if(thisCh('(')) {
    int var = expExpr();
    expThisCh(')');
    return var;
  } else {
    int i = lexNum();
    if(i != -1) return constNum(TY_I32, i);
    else {
      char* buf = getCursor();
      int len = lexIdent();
      if(!len) return -1;
      if(thisCh('(')) {
        int args[MAX_ARGS];
        int c = 0;
        args[c] = expr();
        if(args[c] >= 0) c++;
        while(thisCh(',')) {
          args[c] = expExpr();
          c++;
        }
        expThisCh(')');
        return funcCall(buf, len, args, c);
      }
      return defVar(buf, len, TY_I32);
    }
  }
}

int expExprFactor() {
  int res = exprFactor();
  if(res < 0) panicParse("exprFactor");
  return res;
}

int exprSuf() {
  int var = exprFactor();

  int o;
  while((o = theseOp(group_suf, count_suf))) var = unaryOp(o, var);

  return var;
}

int expExprSuf() {
  int res = exprSuf();
  if(res < 0) panicParse("exprSuf");
  return res;
}

int exprUnary() {
  int o = theseOp(group_unary, count_unary);
  int var = exprSuf();
  if(var < 0) {
    if(o) panicParse("exprUnary");
    else return -1;
  }

  if(o) var = unaryOp(o, var);
  return var;
}

int expExprUnary() {
  int res = exprUnary();
  if(res < 0) panicParse("exprUnary");
  return res;
}

int exprMul() {
  int var = exprUnary();
  if(var < 0) return -1;

  int o;
  while((o = theseOp(group_mul, count_mul))) var = binOp(o, var, expExprUnary());

  return var;
}

int expExprMul() {
  int res = exprMul();
  if(res < 0) panicParse("exprMul");
  return res;
}

int exprAdd() {
  int var = exprMul();
  if(var < 0) return -1;

  int o;
  while((o = theseOp(group_add, count_add))) var = binOp(o, var, expExprMul());

  return var;
}

int expExprAdd() {
  int res = exprAdd();
  if(res < 0) panicParse("exprAdd");
  return res;
}

int exprCmp() {
  int var = exprAdd();
  if(var < 0) return -1;

  int o;
  while((o = theseOp(group_cmp, count_cmp))) var = binOp(o, var, expExprAdd());

  return var;
}

int expExprCmp() {
  int res = exprCmp();
  if(res < 0) panicParse("exprCmp");
  return res;
}

int exprEq() {
  int var = exprCmp();
  if(var < 0) return -1;

  int o;
  while((o = theseOp(group_eq, count_eq))) var = binOp(o, var, expExprCmp());

  return var;
}

int expExprEq() {
  int res = exprEq();
  if(res < 0) panicParse("exprEq");
  return res;
}

int exprAnd() {
  int var = exprEq();
  if(var < 0) return -1;

  int o;
  while((o = theseOp(group_and, count_and))) var = binOp(o, var, expExprEq());

  return var;
}

int expExprAnd() {
  int res = exprAnd();
  if(res < 0) panicParse("exprAnd");
  return res;
}

int exprOr() {
  int var = exprAnd();
  if(var < 0) return -1;

  int o;
  while((o = theseOp(group_or, count_or))) var = binOp(o, var, expExprAnd());

  return var;
}

int expExprOr() {
  int res = exprOr();
  if(res < 0) panicParse("exprOr");
  return res;
}

int expExprAsg();

int exprAsg() {
  int var = exprOr();

  int o = theseOp(group_asg, count_asg);
  if(o) var = binOp(o, var, expExprAsg());

  return var;
}

int expExprAsg() {
  int res = exprAsg();
  if(res < 0) panicParse("exprAsg");
  return res;
}

int expr() {
  return exprAsg();
}

int expExpr() {
  int res = expr();
  if(res < 0) panicParse("expr");
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

int defFunc() {
  char* name = getCursor();
  int len = lexIdent();
  if(!len) return 0;
  expThisCh('(');

  char* args[MAX_ARGS];
  int arg_lens[MAX_ARGS];
  int arg_tys[MAX_ARGS];
  int c = 0;
  args[c] = getCursor();
  arg_lens[c] = lexIdent();
  arg_tys[c] = TY_I32;
  if(arg_lens[c]) c++;
  while(thisCh(',')) {
    args[c] = getCursor();
    arg_lens[c] = expIdent();
    arg_tys[c] = TY_I32;
    c++;
  }
  expThisCh(')');

  funcBegin(name, len, TY_I32, args, arg_lens, arg_tys, c);
  expStmt();
  funcEnd();
  return 1;
}

void program() {
  while(defFunc()) {};
}

void parse() {
  program();

  if(!atEof()) {
    ePutStrLn("WARN: Unread characters are remain.");
  }
}
