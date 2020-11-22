#include "teal.h"

#define MAX_INPUT 50000

int main() {
  // Read STDIN.
  char buf[MAX_INPUT + 1];
  read_stdin(buf, MAX_INPUT);

  // debug: Print the buffer.
  // fprintf(stderr, "%s", buf);

  // debug: Print a receved number.
  // fprintf(stderr, "%d\n", n);

  init_lexer(buf);

  // Write assembly.
  puts(".intel_syntax noprefix");
  puts(".globl main");
  puts("main:");

  // Read a number.
  put("  mov rax, ");
  putlen(exp_num());
  putc('\n');

  // Addition and subtraction.
  while (!eof()) {
    int o = exp_op();
    if (o == OP_ADD) {
      put("  add rax, ");
      putlen(exp_num());
      putc('\n');
    } else {
      put("  sub rax, ");
      putlen(exp_num());
      putc('\n');
    }
  }

  puts("  ret");

  return 0;
}
