#include"teal.h"

char* buf;

void init_lexer(char *buf2) {
  buf = buf2;
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
  if(c == '+') {
    return OP_ADD;
  } else if (c == '-') {
    return OP_SUB;
  } else if (c == '*') {
    return OP_MUL;
  } else if (c == '/') {
    return OP_DIV;
  } else {
    buf--;
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
  int b = op() == o;
  if (!b) {
    buf--;
  }

  return b;
}

void exp_this_op(int o) {
  if(!this_op(o)) {
    panic();
  }
}
