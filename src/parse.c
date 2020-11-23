#include"teal.h"

void expr() {
  inst1("push", exp_num());

  int o;
  while((o = op())) {
    inst1("push", exp_num());
    inst1("pop", REG_RDI);
    inst1("pop", REG_RAX);

    if(o == OP_ADD) {
      inst2("add", REG_RAX, REG_RDI);
    } else if (o == OP_SUB) {
      inst2("sub", REG_RAX, REG_RDI);
    } else {
      eputs("Unknown operator");
      sys_exit(1);
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
