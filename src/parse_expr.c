#include"teal.h"

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
  int ops[2] = {OP_INC, OP_DEC};
  int opc = 2;

  int var = exprFactor();

  int o;
  while((o = theseOp(ops, opc))) var = unaryOp(o, var);

  return var;
}

int expExprSuf() {
  int res = exprSuf();
  if(res < 0) panicParse("exprSuf");
  return res;
}

int exprUnary() {
  int ops[3] = {OP_ADD, OP_SUB, OP_NOT};
  int opc = 3;

  int o = theseOp(ops, opc);
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
  int ops[3] = {OP_MUL, OP_DIV, OP_REM};
  int opc = 3;

  int var = exprUnary();
  if(var < 0) return -1;

  int o;
  while((o = theseOp(ops, opc))) var = binOp(o, var, expExprUnary());

  return var;
}

int expExprMul() {
  int res = exprMul();
  if(res < 0) panicParse("exprMul");
  return res;
}

int exprAdd() {
  int ops[2] = {OP_ADD, OP_SUB};
  int opc = 2;

  int var = exprMul();
  if(var < 0) return -1;

  int o;
  while((o = theseOp(ops, opc))) var = binOp(o, var, expExprMul());

  return var;
}

int expExprAdd() {
  int res = exprAdd();
  if(res < 0) panicParse("exprAdd");
  return res;
}

int exprCmp() {
  int ops[4] = {OP_LE, OP_LT, OP_GE, OP_GT};
  int opc = 4;

  int var = exprAdd();
  if(var < 0) return -1;

  int o;
  while((o = theseOp(ops, opc))) var = binOp(o, var, expExprAdd());

  return var;
}

int expExprCmp() {
  int res = exprCmp();
  if(res < 0) panicParse("exprCmp");
  return res;
}

int exprEq() {
  int ops[2] = {OP_EQ, OP_NE};
  int opc = 2;

  int var = exprCmp();
  if(var < 0) return -1;

  int o;
  while((o = theseOp(ops, opc))) var = binOp(o, var, expExprCmp());

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
  while((o = thisOp(OP_AND))) var = binOp(o, var, expExprEq());

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
  while((o = thisOp(OP_OR))) var = binOp(o, var, expExprAnd());

  return var;
}

int expExprOr() {
  int res = exprOr();
  if(res < 0) panicParse("exprOr");
  return res;
}

int expExprAsg();

int exprAsg() {
  int ops[6] = {OP_ASG, OP_ADDASG, OP_SUBASG, OP_MULASG, OP_DIVASG, OP_REMASG};
  int opc = 6;

  int var = exprOr();

  int o = theseOp(ops, opc);
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

