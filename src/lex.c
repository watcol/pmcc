#include"teal.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

char* buf;

void init_lexer(char *buf2) {
  buf = buf2;
}

void space() {
  while(isspace(*buf)) {
    buf++;
  }
}

int num() {
  space();
  if(isdigit(*buf)) {
    return strtol(buf, &buf, 10);
  } else {
    return -1;
  }
}

int exp_num() {
  space();
  if(isdigit(*buf)) {
    return strtol(buf, &buf, 10);
  } else {
    fprintf(stderr, "Unexpected token.");
    exit(1);
  }
}

int eof() {
  space();
  return !*buf;
}

void exp_eof() {
  if(!eof()) {
    fprintf(stderr, "Unexpected token.");
    exit(1);
  }
}

int op() {
  space();
  if(*buf == '+') {
    buf++;
    return LEX_ADD;
  } else if (*buf == '-') {
    buf++;
    return LEX_SUB;
  } else {
    return LEX_EOF;
  }
}

int exp_op() {
  int o = op();
  if(o == LEX_EOF) {
    fprintf(stderr, "Unexpected token.");
    exit(1);
  }

  return o;
}

int this_op(int o) {
  return op() == o;
}

void exp_this_op(int o) {
  if(!this_op(o)) {
    fprintf(stderr, "Unexpected token.");
    exit(1);
  }
}
