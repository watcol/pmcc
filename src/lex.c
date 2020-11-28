#include"teal.h"

char* buf;
int tmp;

void init_lexer(char *buf2) {
  buf = buf2;
  tmp = 0;
}

void lex_put() {
  write(buf, tmp);
  buf+=tmp;
  tmp = 0;
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

void skip_space() {
  while(is_space(*buf) || comment()) {
    buf++;
  }
}

void panic() {
  eputs("Unexpected token.");
  sys_exit(1);
}

char lex_ch() {
  skip_space();
  char c = *buf;
  buf++;
  tmp = 1;
  return c;
}

char this_ch(char c) {
  if(lex_ch() == c) {
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

int lex_num() {
  skip_space();
  if(is_digit(*buf)) {
    tmp = digitlen(buf);
    return VAL_LEX;
  } else {
    return VAL_UNKNOWN;
  }
}

int exp_num() {
  int i = lex_num();
  if(!i) {
    panic();
  }
  return i;
}

int lex_ident() {
  skip_space();
  if(is_alpha(*buf)) {
    int offset = (*buf - 'a') * 8;
    buf++;
    return -offset;
  } else {
    return VAL_UNKNOWN;
  }
}

int exp_ident() {
  int i = lex_ident();
  if(!i) {
    panic();
  }
  return i;
}

int at_eof() {
  skip_space();
  return !*buf;
}

void exp_eof() {
  if(!at_eof()) {
    panic();
  }
}

int lex_op() {
  skip_space();
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
      return OP_UNKNOWN;
    }
  } else if(c == '!') {
    if(*buf == '=') {
      buf++;
      tmp = 2;
      return OP_NE;
    } else {
      buf--;
      tmp = 0;
      return OP_UNKNOWN;
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
    return OP_UNKNOWN;
  }
}

int exp_op() {
  int o = lex_op();
  if(!o) {
    panic();
  }

  return o;
}

int this_op(int o) {
  if(lex_op() != o) {
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
