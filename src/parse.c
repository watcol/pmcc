#include"teal.h"

int group0[] = {OP_ADD, OP_SUB};
int count0 = 2;

int group1[] = {OP_MUL, OP_DIV};
int count1 = 2;

void expr();

void expr_fact() {
  if(this_ch('(')) {
    expr();
    exp_this_ch(')');
  } else {
    inst1("push", exp_num());
  }
}

void expr1() {
  expr_fact();

  int o;
  while((o = these_op(group1, count1))) {
    expr_fact();
    inst1("pop", REG_RDI);
    inst1("pop", REG_RAX);

    if(o == OP_MUL) {
      inst2("imul", REG_RAX, REG_RDI);
    } else if (o == OP_DIV) {
      inst("cqo");
      inst1("idiv", REG_RDI);
    } else {
      eputs("Unknown operator");
      sys_exit(1);
    }

    inst1("push", REG_RAX);
  }
}

void expr() {
  expr1();

  int o;
  while((o = these_op(group0, count0))) {
    expr1();
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
