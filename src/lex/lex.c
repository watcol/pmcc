#include"lex.h"
#define MAX_BUFFER 50000

char buf[MAX_BUFFER + 1];
char* cur;
int tmp;

void initLexer() {
  readStdin(buf, MAX_BUFFER);
  cur = buf;
  tmp = 0;
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

int lexOp() {
  skipSpace();
  tmp = 0;

  if(*cur == '+') {
    cur++;
    tmp++;
    if(*cur == '+') {
      cur++;
      tmp++;
      return OP_INC;
    } else if (*cur == '=') {
      cur++;
      tmp++;
      return OP_ADDASG;
    } else {
      return OP_ADD;
    }
  } else if (*cur == '-') {
    cur++;
    tmp++;
    if(*cur == '-') {
      cur++;
      tmp++;
      return OP_DEC;
    } else if (*cur == '=') {
      cur++;
      tmp++;
      return OP_SUBASG;
    } else {
      return OP_SUB;
    }
  } else if (*cur == '*') {
    cur++;
    tmp++;
    if(*cur == '=') {
      cur++;
      tmp++;
      return OP_MULASG;
    } else {
      return OP_MUL;
    }
  } else if (*cur == '/') {
    cur++;
    tmp++;
    if(*cur == '=') {
      cur++;
      tmp++;
      return OP_DIVASG;
    } else {
      return OP_DIV;
    }
  } else if (*cur == '%') {
    cur++;
    tmp++;
    if(*cur == '=') {
      cur++;
      tmp++;
      return OP_REMASG;
    } else {
      return OP_REM;
    }
  } else if (*cur == '=') {
    cur++;
    tmp++;
    if (*cur == '=') {
      cur++;
      tmp++;
      return OP_EQ;
    } else {
      return OP_ASG;
    }
  } else if(*cur == '!') {
    cur++;
    tmp++;
    if(*cur == '=') {
      cur++;
      tmp++;
      return OP_NE;
    } else {
      return OP_NOT;
    }
  } else if(*cur == '<') {
    cur++;
    tmp++;
    if (*cur == '=') {
      cur++;
      tmp++;
      return OP_LE;
    } else {
      return OP_LT;
    }
  } else if(*cur == '>') {
    cur++;
    tmp++;
    if (*cur == '=') {
      cur++;
      tmp++;
      return OP_GE;
    } else {
      return OP_GT;
    }
  } else if(*cur == '&') {
    cur++;
    tmp++;
    if (*cur == '&') {
      cur++;
      tmp++;
      return OP_AND;
    } else {
      cur--;
      tmp--;
      return OP_UNKNOWN;
    }
  } else if(*cur == '|') {
    cur++;
    tmp++;
    if (*cur == '|') {
      cur++;
      tmp++;
      return OP_OR;
    } else {
      cur--;
      tmp--;
      return OP_UNKNOWN;
    }
  } else {
    return OP_UNKNOWN;
  }
}

int expOp() {
  int o = lexOp();
  if(!o) {
    panicLex();
  }

  return o;
}

int thisOp(int o) {
  if(lexOp() != o) {
    cur-=tmp;
    return 0;
  } else {
    return o;
  }
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
