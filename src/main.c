#include "teal.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_INPUT 50000
#define MAX_TOKEN 1000

int main() {
  // Read STDIN.
  char buf2[MAX_INPUT + 1] = "";
  read_stdin(buf2, MAX_INPUT);

  // debug: Print the buffer.
  // fprintf(stderr, "%s", buf);

  // Convert char[] to char*
  char *buf = buf2;

  // debug: Print a receved number.
  // fprintf(stderr, "%d\n", n);

  int tokens[MAX_TOKEN];
  char* start[MAX_TOKEN];
  size_t len[MAX_TOKEN];

  buf2tokens(buf, tokens, start, len, MAX_TOKEN);

  // Write assembly.
  puts(".intel_syntax noprefix");
  puts(".globl main");
  puts("main:");

  // Read a number.
  if (tokens[0] != LEX_NUM) {
    fprintf(stderr, "The first token should be a number.");
  }
  printf("  mov rax, %ld\n", strtol(copy_range(start[0], len[0]), NULL, 10));

  // Addition and subtraction.
  for(int i = 1; tokens[i] != LEX_EOF; i++) {
    if (tokens[i] != LEX_SYMBOL) {
      fprintf(stderr, "The token after a number should be a symbol.");
    }

    char* sym = copy_range(start[i], len[i]);
    i++;

    if (tokens[i] != LEX_NUM) {
      fprintf(stderr, "The token after a symbol should be a number.");
    }

    if(*sym == '+') {
      printf("  add rax, %ld\n", strtol(copy_range(start[i], len[i]), NULL, 10));
    } else if(*sym == '-') {
      printf("  sub rax, %ld\n", strtol(copy_range(start[i], len[i]), NULL, 10));
    }
  }

  puts("  ret");

  return 0;
}
