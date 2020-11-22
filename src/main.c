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

  init_lexer(buf);

  // Write assembly.
  puts(".intel_syntax noprefix");
  puts(".globl main");
  puts("main:");

  // Read a number.
  if (token(0) != LEX_NUM) {
    fprintf(stderr, "The first token should be a number.");
  }
  printf("  mov rax, %ld\n", strtol(value(0), NULL, 10));

  // Addition and subtraction.
  for(int i = 1; token(i) != LEX_EOF; i++) {
    int sym = token(i);
    if (sym != LEX_ADD && sym != LEX_SUB) {
      fprintf(stderr, "The token after a number should be a symbol.");
    }

    i++;
    if (token(i) != LEX_NUM) {
      fprintf(stderr, "The token after a symbol should be a number.");
    }

    if(sym == LEX_ADD) {
      printf("  add rax, %ld\n", strtol(value(i), NULL, 10));
    } else {
      printf("  sub rax, %ld\n", strtol(value(i), NULL, 10));
    }
  }

  puts("  ret");

  return 0;
}
