#include"lex.h"
#define MAX_STR 20

int getCh(int str) {
  char c = getChInner();
  if(c == '\\') {
    char d = getChInner();
    if(d == '0') return '\0';
    if(d == 'a') return '\a';
    if(d == 'b') return '\b';
    if(d == 'f') return '\f';
    if(d == 'n') return '\n';
    if(d == 'r') return '\r';
    if(d == 't') return '\t';
    if(d == 'v') return '\v';
    if(d == '\\') return '\\';
    if(d == '\'') return '\'';
    if(d == '\"') return '\"';
    panicLex();
    return -1;
  } else if(c == '\'' && !str) {
    consume(-1);
    return -1;
  } else if(c == '\"' && str) {
    consume(-1);
    return -1;
  } else {
    return c;
  }
}

char expCh() {
  int c = getCh(0);
  if(c < 0) panicLex();
  return c;
}

int getStr1() {
  return getCh(1);
}
