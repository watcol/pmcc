#include"teal.h"

void expr() {
  inst2("mov", REG_RAX, exp_num());

  while(!eof()) {
    int o = exp_op();
    if(o == OP_ADD) {
      inst2("add", REG_RAX, exp_num());
    } else {
      inst2("sub", REG_RAX, exp_num());
    }
  }
}

void parse(char* buf) {
  init_lexer(buf);

  init_code();
  func("main");

  expr();

  inst("ret");
}
