#include"lex.h"

int atEof() {
  char* cur = getCursor();
  return !*cur;
}

void expEof() {
  if(!atEof()) {
    panicLex();
  }
}
