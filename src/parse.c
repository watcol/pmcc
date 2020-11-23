#include"teal.h"

void expr() {
  inst1("push", exp_num());

  while(!eof()) {
    int o = exp_op();
    inst1("push", exp_num());
    inst1("pop", REG_RDI);
    inst1("pop", REG_RAX);

    if(o == OP_ADD) {
      inst2("add", REG_RAX, REG_RDI);
    } else {
      inst2("sub", REG_RAX, REG_RDI);
    }

    inst1("push", REG_RAX);
  }
}



void parse(char* buf) {
  init_lexer(buf);

  init_code();
  func("main");

  expr();

  inst1("pop", REG_RAX);
  inst("ret");
}
