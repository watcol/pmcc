#include"lex.h"

char* ops[25] = {
  "unknown",
  "+", "-", "*", "/", "%",
  "<", "<=", ">", ">=",
  "==", "!=", "!",
  "&&", "||",
  "++", "--",
  "=", "+=", "-=", "*=", "/=", "%=",
  "*", "&"
};

int thisOpInner(int o) {
  int danger = (o >= OP_ADD && o <= OP_REM)
             || o == OP_LT
             || o == OP_GT
             || o == OP_DEREF;

  if(danger) {
    if(!thisStr(ops[o])) return 0;
    if(thisCh('=')) {
      consume(-2);
      return 0;
    }

    return 1;
  } else {
    return thisStr(ops[o]);
  }
}

int thisOp(int o) {
  if(o <= 0 || o > 24) panicLex();
  return thisOpInner(o);
}

int theseOp(int* os, int c) {
  int i = 0;
  while(i < c) {
    if(thisOp(os[i])) {
      return os[i];
    }
    i++;
  }

  return 0;
}

int expThisOp(int o) {
  if(!thisOp(o)) {
    panicLex();
  }

  return o;
}

int expTheseOp(int* os, int c) {
  int o = theseOp(os, c);
  if(!o) {
    panicLex();
  }

  return o;
}
