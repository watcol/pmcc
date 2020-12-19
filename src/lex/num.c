#include"lex.h"

int lexNum() {
  char* cur = getCursor();
  char* e;
  int res = strToI(cur, &e);
  consume(e - cur);
  return res;
}

int expNum() {
  int i = lexNum();
  if(i == -1) {
    panicLex();
  }
  return i;
}
