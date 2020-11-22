#include"teal.h"
#include<stdio.h>
#include<stdlib.h>

char* buf;

void init_lexer(char *buf2) {
  buf = buf2;
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

_Noreturn void panic() {
  fprintf(stderr, "Unexpected token.");
  exit(1);
}

int num() {
  space();
  if(is_digit(*buf)) {
    return strtol(buf, &buf, 10);
  } else {
    return -1;
  }
}

int exp_num() {
  space();
  if(is_digit(*buf)) {
    return strtol(buf, &buf, 10);
  } else {
    panic();
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
  if(*buf == '+') {
    buf++;
    return OP_ADD;
  } else if (*buf == '-') {
    buf++;
    return OP_SUB;
  } else {
    return -1;
  }
}

int exp_op() {
  int o = op();
  if(o == -1) {
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
