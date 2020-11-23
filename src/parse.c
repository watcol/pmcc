#include"teal.h"

int group_add[] = {OP_ADD, OP_SUB};
int count_add = 2;

int group_mul[] = {OP_MUL, OP_DIV};
int count_mul = 2;

void expr();

void expr_factor() {
  if(this_ch('(')) {
    expr();
    exp_this_ch(')');
  } else {
    inst1("push", exp_num());
  }
}

void expr_unary() {
  int o = these_op(group_add, count_add);
  expr_factor();
  if(o == OP_SUB) {
    inst1("pop", REG_RAX);
    inst2v("imul", REG_RAX, "-1");
    inst1("push", REG_RAX);
  }
}

void expr_mul() {
  expr_unary();

  int o;
  while((o = these_op(group_mul, count_mul))) {
    expr_unary();
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

void expr_add() {
  expr_mul();

  int o;
  while((o = these_op(group_add, count_add))) {
    expr_mul();
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

void expr() {
  expr_add();
}

void parse(char* buf) {
  init_lexer(buf);

  init_code();
  func("main");

  expr();

  inst1("pop", REG_RAX);
  inst("ret");
}
