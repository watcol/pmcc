#include"lex.h"

char* ops[23] = {
  "unknown",
  "+", "-", "*", "/", "%",
  "<", "<=", ">", ">=",
  "==", "!=", "!",
  "&&", "||",
  "++", "--",
  "=", "+=", "-=", "*=", "/=", "%="
};

int thisOp(int o) {
  if(o <= 0 || o > 22) panicLex();
  return thisStr(ops[o]);
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
