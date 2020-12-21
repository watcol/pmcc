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

int thisStr(char* str) {
  char* cur = getCursor();
  int len = length(str);
  if (strCmp(cur, str, len)) {
    consume(len);
    return 1;
  }

  return 0;
}

int expThisStr(char* str) {
  int len = thisStr(str);
  if(!len) {
    panicLex();
    return 0;
  }

  return len;
}
