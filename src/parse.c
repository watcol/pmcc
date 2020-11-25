#include"teal.h"

int group_add[2] = {OP_ADD, OP_SUB};
int count_add = 2;

int group_mul[2] = {OP_MUL, OP_DIV};
int count_mul = 2;

int group_cmp[4] = {OP_L, OP_LE, OP_ME, OP_M};
int count_cmp = 4;

int group_eq[2] = {OP_E, OP_NE};
int count_eq = 2;

void expr();

void expr_factor() {
  if(this_ch('(')) {
    expr();
    exp_this_ch(')');
  } else {
    instv("push", exp_num());
  }
}

void expr_unary() {
  int o = these_op(group_add, count_add);
  expr_factor();
  if(o == OP_SUB) {
    instv("pop", VAL_RAX);
    instvs("imul", VAL_RAX, "-1");
    instv("push", VAL_RAX);
  }
}

void expr_mul() {
  expr_unary();

  int o;
  while((o = these_op(group_mul, count_mul))) {
    expr_unary();
    instv("pop", VAL_RDI);
    instv("pop", VAL_RAX);

    if(o == OP_MUL) {
      instvv("imul", VAL_RAX, VAL_RDI);
    } else if (o == OP_DIV) {
      inst("cqo");
      instv("idiv", VAL_RDI);
    } else {
      eputs("Unknown operator");
      sys_exit(1);
    }

    instv("push", VAL_RAX);
  }
}

void expr_add() {
  expr_mul();

  int o;
  while((o = these_op(group_add, count_add))) {
    expr_mul();
    instv("pop", VAL_RDI);
    instv("pop", VAL_RAX);

    if(o == OP_ADD) {
      instvv("add", VAL_RAX, VAL_RDI);
    } else if (o == OP_SUB) {
      instvv("sub", VAL_RAX, VAL_RDI);
    } else {
      eputs("Unknown operator");
      sys_exit(1);
    }

    instv("push", VAL_RAX);
  }
}

void expr_cmp() {
  expr_add();

  int o;
  while((o = these_op(group_cmp, count_cmp))) {
    expr_add();
    instv("pop", VAL_RDI);
    instv("pop", VAL_RAX);

    if(o == OP_L) {
      instvv("cmp", VAL_RAX, VAL_RDI);
      instv("setl", VAL_AL);
    } else if(o == OP_LE) {
      instvv("cmp", VAL_RAX, VAL_RDI);
      instv("setle", VAL_AL);
    } else if(o == OP_ME) {
      instvv("cmp", VAL_RDI, VAL_RAX);
      instv("setle", VAL_AL);
    } else if(o == OP_M) {
      instvv("cmp", VAL_RDI, VAL_RAX);
      instv("setl", VAL_AL);
    }

    instvv("movzb", VAL_RAX, VAL_AL);
    instv("push", VAL_RAX);
  }
}

void expr_eq() {
  expr_cmp();

  int o;
  while((o = these_op(group_eq, count_eq))) {
    expr_cmp();
    instv("pop", VAL_RDI);
    instv("pop", VAL_RAX);
    instvv("cmp", VAL_RAX, VAL_RDI);

    if(o == OP_E) {
      instv("sete", VAL_AL);
    } else if(o == OP_NE) {
      instv("setne", VAL_AL);
    }

    instvv("movzb", VAL_RAX, VAL_AL);
    instv("push", VAL_RAX);
  }
}

void expr() {
  expr_eq();
}

void parse(char* buf) {
  init_lexer(buf);

  init_code();
  func("main");

  expr();

  instv("pop", VAL_RAX);
  inst("ret");
}
