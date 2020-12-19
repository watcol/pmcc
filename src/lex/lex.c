#include"lex.h"
#define MAX_BUFFER 50000

char buf[MAX_BUFFER + 1];
char* cur;

char* ops[23] = {
  "unknown",
  "+", "-", "*", "/", "%",
  "<", "<=", ">", ">=",
  "==", "!=", "!",
  "&&", "||",
  "++", "--",
  "=", "+=", "-=", "*=", "/=", "%="
};

void initLexer() {
  readStdin(buf, MAX_BUFFER);
  cur = buf;
}

char* getCursor() {
  skipSpace();
  return cur;
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

void panicLex() {
  ePutStr("Unexpected token: '");
  ePutCh(*cur);
  ePutStrLn("'.");
  sysExit(1);
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

int thisOp(int o) {
  if(o <= 0 || o > 22) panicLex();
  return thisStr(ops[o]);
}

int theseOp(int* os, int c) {
  int i = 0;
  while(i < c) {
    if(thisOp(os[i])) {
      return os[i];
    }
    i++;
  }

  return 0;
}

int expThisOp(int o) {
  if(!thisOp(o)) {
    panicLex();
  }

  return o;
}

int expTheseOp(int* os, int c) {
  int o = theseOp(os, c);
  if(!o) {
    panicLex();
  }

  return o;
}
