#include "teal.h"

#define MAX_INPUT 50000

int main() {
  // Read STDIN.
  char buf2[MAX_INPUT + 1];
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
  put("  mov rax, ");
  put_len(exp_num());
  putc('\n');

  // Addition and subtraction.
  while (!eof()) {
    int o = exp_op();
    if (o == OP_ADD) {
      put("  add rax, ");
      put_len(exp_num());
      putc('\n');
    } else {
      put("  sub rax, ");
      put_len(exp_num());
      putc('\n');
    }
  }

  puts("  ret");

  return 0;
}
