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

int thisIdent(char* str) {
  char* cur = getCursor();
  int len = identLen(cur);
  int len2 = length(str);
  if(len != len2) return 0;
  if(strCmp(str, cur, len)) {
    consume(len);
    return 1;
  }

  return 0;
}

void expThisIdent(char* str) {
  int i = thisIdent(str);
  if(!i) {
    panicLex();
  }
}
