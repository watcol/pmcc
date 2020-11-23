#include"teal.h"

char* buf;
int tmp;

void init_lexer(char *buf2) {
  buf = buf2;
  tmp = 0;
}

void putlen(int len) {
  write(buf, len);
  buf+=len;
}

int comment() {
  if(!(*buf == '/' && *(buf+1) == '/')) {
    return 0;
  }

  while(*buf != '\n') {
    buf++;
  }

  return 1;
}

void space() {
  while(is_space(*buf) || comment()) {
    buf++;
  }
}

void panic() {
  eputs("Unexpected token.");
  sys_exit(1);
}

char ch() {
  space();
  char c = *buf;
  buf++;
  tmp = 1;
  return c;
}

char this_ch(char c) {
  if(ch() == c) {
    return c;
  } else {
    buf-=tmp;
    return 0;
  }
}

char exp_this_ch(char c) {
  if(!this_ch(c)) {
    panic();
  }
  return c;
}

int num() {
  space();
  if(is_digit(*buf)) {
    return digitlen(buf);
  } else {
    return -1;
  }
}

int exp_num() {
  int len = num();
  if(len == -1) {
    panic();
    return -1;
  } else {
    return len;
  }
}

int eof() {
  space();
  return !*buf;
}

void exp_eof() {
  if(!eof()) {
    panic();
  }
}

int op() {
  space();
  char c = *buf;
  buf++;
  tmp = 1;
  if(c == '+') {
    return OP_ADD;
  } else if (c == '-') {
    return OP_SUB;
  } else if (c == '*') {
    return OP_MUL;
  } else if (c == '/') {
    return OP_DIV;
  } else if (c == '=') {
    if (*buf == '=') {
      buf++;
      tmp = 2;
      return OP_E;
    } else {
      buf--;
      tmp = 0;
      return 0;
    }
  } else if(c == '!') {
    if(*buf == '=') {
      buf++;
      tmp = 2;
      return OP_NE;
    } else {
      buf--;
      tmp = 0;
      return 0;
    }
  } else if(c == '<') {
    if (*buf == '=') {
      buf++;
      tmp = 2;
      return OP_LE;
    } else {
      return OP_L;
    }
  } else if(c == '>') {
    if (*buf == '=') {
      buf++;
      tmp = 2;
      return OP_ME;
    } else {
      return OP_M;
    }
  } else {
    buf--;
    tmp = 0;
    return 0;
  }
}

int exp_op() {
  int o = op();
  if(!o) {
    panic();
  }

  return o;
}

int this_op(int o) {
  if(op() != o) {
    buf-=tmp;
    return 0;
  } else {
    return o;
  }
}

int these_op(int* os, int c) {
  int i = 0;
  while(i < c) {
    if(this_op(os[i])) {
      return os[i];
    }
    i++;
  }

  return 0;
}

int exp_this_op(int o) {
  if(!this_op(o)) {
    panic();
  }

  return o;
}

int exp_these_op(int* os, int c) {
  int o = these_op(os, c);
  if(!o) {
    panic();
  }

  return o;
}
