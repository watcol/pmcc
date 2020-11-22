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
  printf("  mov rax, %d\n", exp_num());

  // Addition and subtraction.
  while(!eof()) {
    int o = exp_op();
    if(o == LEX_ADD) {
      printf("  add rax, %d\n", exp_num());
    } else {
    printf("  sub rax, %d\n", exp_num());
    }
  }

  puts("  ret");

  return 0;
}
