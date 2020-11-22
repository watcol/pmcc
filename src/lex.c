#include"teal.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

void buf2tokens(char *buf, int *tokens, char **start, size_t *len, size_t count) {
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
    if (c == count) {
      fprintf(stderr, "Too many tokens");
      exit(1);
    }
  }

  tokens[c] = LEX_EOF;
  start[c] = buf;
  len[c] = 0;
}

char* copy_range(char* start, size_t len) {
  char *ret = malloc(len+1);
  memcpy(ret, start, len);
  ret[len] = '\0';
  return ret;
}
