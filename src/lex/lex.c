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

char getChInner() {
  char c = *cur;
  consume(1);
  return c;
}

void consume(int len) {
  cur+=len;
}
