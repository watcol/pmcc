#include"teal.h"
#define MAX_MARKER 100
#define MAX_VARS 30

char* buf;
char* marker[MAX_MARKER];
int tmp;
char* vars[MAX_VARS];
int var_num;

void init_lexer(char *buf2) {
  buf = buf2;

  int c = 0;
  while(c < MAX_MARKER) {
    marker[c] = NULL;
    c++;
  }

  var_num = 0;
  c = 0;
  while(c < MAX_VARS) {
    vars[c] = NULL;
    c++;
  }

  tmp = 0;
}

int mark() {
  int c = 0;
  while(marker[c] != NULL && c != MAX_MARKER) {
    c++;
  }

  if(c == MAX_MARKER) {
    eputs("Too many markers.");
    sys_exit(1);
  }

  marker[c] = buf;
  return c;
}

void jump(int c) {
  if(marker[c] == NULL) {
    eputs("Use of unavailable marker.");
    sys_exit(1);
  }

  buf = marker[c];
  marker[c] = NULL;
}

void unmark(int c) {
  marker[c] = NULL;
}

void lex_put() {
  write(buf, tmp);
  buf+=tmp;
  tmp = 0;
}

int get_offset() {
  int c = 0;
  while(!str_cmp(buf, vars[c], tmp) && c != var_num) {
    c++;
  }

  if(c == var_num) {
    if(var_num == MAX_VARS) {
      eputs("Too many variables");
      sys_exit(1);
    }

    var_num++;
    vars[c] = buf;
  }

  buf += tmp;
  return c * 8;
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

int this_str(char* str) {
  skip_space();
  int len = length(str);
  if (str_cmp(buf, str, len)) {
    buf+=len;
    return 1;
  }

  return 0;
}

int exp_this_str(char* str) {
  int len = this_str(str);
  if(!len) {
    panic();
    return 0;
  }

  return len;
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
    tmp = identlen(buf);
    return -(get_offset() + 1);
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
      return OP_EQ;
    } else {
      return OP_ASG;
    }
  } else if(c == '!') {
    if(*buf == '=') {
      buf++;
      tmp = 2;
      return OP_NEQ;
    } else {
      buf--;
      tmp = 0;
      return OP_UNKNOWN;
    }
  } else if(c == '<') {
    if (*buf == '=') {
      buf++;
      tmp = 2;
      return OP_LEQ;
    } else {
      return OP_LESS;
    }
  } else if(c == '>') {
    if (*buf == '=') {
      buf++;
      tmp = 2;
      return OP_MEQ;
    } else {
      return OP_MORE;
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
