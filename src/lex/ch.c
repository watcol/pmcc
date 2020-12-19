#include"lex.h"

char thisCh(char c) {
  char* cur = getCursor();
  if(*cur == c) {
    consume(1);
    return c;
  }

  return '\0';
}

char expThisCh(char c) {
  if(!thisCh(c)) {
    panicLex();
  }
  return c;
}
