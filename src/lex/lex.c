#include"lex.h"
#define MAX_BUFFER 50000

char buf[MAX_BUFFER + 1];
char* cur;

void initLexer() {
  readStdin(buf, MAX_BUFFER);
  cur = buf;
}

void panicLex() {
  ePutStr("Unexpected token: '");
  ePutCh(*cur);
  ePutStrLn("'.");
  sysExit(1);
}

int comment() {
  if(!(*cur == '/' && *(cur+1) == '/')) {
    return 0;
  }

  while(*cur != '\n') {
    cur++;
  }

  return 1;
}

void skipSpace() {
  while(isSpace(*cur) || comment()) {
    cur++;
  }
}

char* getCursor() {
  skipSpace();
  return cur;
}

char thisCh(char c) {
  skipSpace();
  if(*cur == c) {
    cur++;
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
  skipSpace();
  int len = length(str);
  if (strCmp(cur, str, len)) {
    cur+=len;
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

int lexNum() {
  skipSpace();
  return strToI(cur, &cur);
}

int expNum() {
  int i = lexNum();
  if(i == -1) {
    panicLex();
  }
  return i;
}

int lexIdent() {
  skipSpace();
  int len = identLen(cur);
  cur+=len;
  return len;
}

int expIdent() {
  int i = lexIdent();
  if(!i) {
    panicLex();
  }
  return i;
}

int atEof() {
  skipSpace();
  return !*cur;
}

void expEof() {
  if(!atEof()) {
    panicLex();
  }
}
