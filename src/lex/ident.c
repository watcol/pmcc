#include"lex.h"

int lexIdent() {
  char* cur = getCursor();
  int len = identLen(cur);
  consume(len);
  return len;
}

int expIdent() {
  int i = lexIdent();
  if(!i) {
    panicLex();
  }
  return i;
}
