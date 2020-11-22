#include"teal.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

#define MAX_TOKEN 1000

int tokens[MAX_TOKEN];
char *start[MAX_TOKEN];
size_t len[MAX_TOKEN];

void init_lexer(char *buf) {
  int c = 0;

  while(*buf) {
    if (isspace(*buf)) {
      // Skip spaces.
      buf++;
      c--;
    } else if (*buf == '+' || *buf == '-') {
      tokens[c] = LEX_SYMBOL;
      start[c] = buf;
      buf++;
      len[c] = 1;
    } else if (isdigit(*buf)) {
      tokens[c] = LEX_NUM;
      start[c] = buf;
      buf++;
      while(isdigit(*buf)) {
        buf++;
      }
      len[c] = buf - start[c];
    } else {
      fprintf(stderr, "Unexpected token: '%c'", *buf);
      exit(1);
    }

    c++;
    if (c == MAX_TOKEN) {
      fprintf(stderr, "Too many tokens");
      exit(1);
    }
  }

  tokens[c] = LEX_EOF;
  start[c] = buf;
  len[c] = 0;
}

int token(int c) {
  return tokens[c];
}

char* value(int c) {
  char* s = start[c];
  size_t l = len[c];
  char *ret = malloc(l+1);
  memcpy(ret, s, l);
  ret[l] = '\0';
  return ret;
}
