#include"lex.h"

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
