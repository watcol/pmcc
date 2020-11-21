#include"teal.h"
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

void buf2tokens(char *buf, int *tokens, char **start, char **end, size_t count) {
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
      end[c] = buf;
    } else if (isdigit(*buf)) {
      tokens[c] = LEX_NUM;
      start[c] = buf;
      buf++;
      while(isdigit(*buf)) {
        buf++;
      }
      end[c] = buf;
    } else {
      fprintf(stderr, "Unexpected token: '%c'", *buf);
      exit(1);
    }

    c++;
    if (c >= count) {
      fprintf(stderr, "Too many tokens");
      exit(1);
    }
  }
}
