#include"teal.h"

int group_add[2] = {OP_ADD, OP_SUB};
int count_add = 2;

int group_mul[2] = {OP_MUL, OP_DIV};
int count_mul = 2;

int group_cmp[4] = {OP_L, OP_LE, OP_ME, OP_M};
int count_cmp = 4;

int group_eq[2] = {OP_E, OP_NE};
int count_eq = 2;

void exp_expr();

int expr_factor() {
  if(this_ch('(')) {
    exp_expr();
    exp_this_ch(')');
    return 1;
  } else {
    int n = lex_num();
    if(n) {
      instv("push", n);
      return 1;
    } else {
      return 0;
    }
  }
}

void exp_expr_factor() {
  if(!expr_factor()) {
    eputs("Parse failed");
    sys_exit(1);
  }
}

int expr_unary() {
  int o = these_op(group_add, count_add);
  if(!expr_factor()) {
    if(o) {
      eputs("Parse failed");
      sys_exit(1);
    } else {
      return 0;
    }
  }

  if(o == OP_SUB) {
    instv("pop", VAL_RAX);
    instvs("imul", VAL_RAX, "-1");
    instv("push", VAL_RAX);
  }

  return 1;
}

void exp_expr_unary() {
  if(!expr_unary()) {
    eputs("Parse failed");
    sys_exit(1);
  }
}

int expr_mul() {
  if(!expr_unary()){
    return 0;
  }

  int o;
  while((o = these_op(group_mul, count_mul))) {
    exp_expr_unary();
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

  return 1;
}

void exp_expr_mul() {
  if(!expr_mul()) {
    eputs("Parse failed");
    sys_exit(1);
  }
}

int expr_add() {
  if(!expr_mul()) {
    return 0;
  }

  int o;
  while((o = these_op(group_add, count_add))) {
    exp_expr_mul();
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

  return 1;
}

void exp_expr_add() {
  if(!expr_add()) {
    eputs("Parse failed");
    sys_exit(1);
  }
}

int expr_cmp() {
  if(!expr_add()) {
    return 0;
  }

  int o;
  while((o = these_op(group_cmp, count_cmp))) {
    exp_expr_add();
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

  return 1;
}

void exp_expr_cmp() {
  if(!expr_cmp()) {
    eputs("Parse failed");
    sys_exit(1);
  }
}

int expr_eq() {
  if(!expr_cmp()) {
    return 0;
  }

  int o;
  while((o = these_op(group_eq, count_eq))) {
    exp_expr_cmp();
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

  return 1;
}

void exp_expr_eq() {
  if(!expr_eq()) {
    eputs("Parse failed");
    sys_exit(1);
  }
}

int expr() {
  return expr_eq();
}

void exp_expr() {
  if(!expr()) {
    eputs("Parse failed");
    sys_exit(1);
  }
}

void parse(char* buf) {
  init_lexer(buf);

  init_code();
  func("main");

  exp_expr();

  instv("pop", VAL_RAX);
  inst("ret");

  if(!at_eof()) {
    eputs("WARN: Unread characters are remain.");
  }
}
