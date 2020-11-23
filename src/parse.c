#include"teal.h"

void expr() {
  put("mov rax, ");
  putlen(exp_num());
  putc('\n');

  while(!eof()) {
    int o = exp_op();
    if(o == OP_ADD) {
      put("  add rax, ");
      putlen(exp_num());
      putc('\n');
    } else {
      put("sub rax, ");
      putlen(exp_num());
      putc('\n');
    }
  }
}

void parse(char* buf) {
  init_lexer(buf);

  puts(".intel_syntax noprefix");
  puts(".globl main");
  puts("main:");

  expr();

  puts("  ret");
}
