#include"teal.h"

int group_suf[2] = {OP_INC, OP_DEC};
int count_suf = 2;

int group_unary[3] = {OP_ADD, OP_SUB, OP_NOT};
int count_unary = 3;

int group_add[2] = {OP_ADD, OP_SUB};
int count_add = 2;

int group_mul[3] = {OP_MUL, OP_DIV, OP_REM};
int count_mul = 3;

int group_cmp[4] = {OP_LESS, OP_LEQ, OP_MEQ, OP_MORE};
int count_cmp = 4;

int group_eq[2] = {OP_EQ, OP_NEQ};
int count_eq = 2;

int group_and[1] = {OP_AND};
int count_and = 1;

int group_or[1] = {OP_OR};
int count_or = 1;

int group_asg[6] = {OP_ASG, OP_ADDASG, OP_SUBASG, OP_MULASG, OP_DIVASG, OP_REMASG};
int count_asg = 6;

void panic_parse(char* at) {
  eput("Parse failed (at \"");
  eput(at);
  eputs("\")");
  sys_exit(1);
}

int lval() {
  return lex_ident();
}

int exp_lval() {
  int l = lval();
  if(!l) {
    panic_parse("lval");
  }

  return l;
}

void exp_expr();

int expr_factor() {
  if(this_ch('(')) {
    exp_expr();
    exp_this_ch(')');
    return 1;
  } else if(lex_num()) {
    pushl();
    return 1;
  } else {
    int l = lval();
    if (l) {
      pushm(l);
      return 1;
    } else {
      return 0;
    }
  }
}

void exp_expr_factor() {
  if(!expr_factor()) {
    panic_parse("expr_factor");
  }
}

int expr_suf() {
  int m = mark();
  int l = lval();
  if(!l) {
    unmark(m);
    return expr_factor();
  }

  int o = these_op(group_suf, count_suf);
  if(o) {
    unmark(m);
    if (o == OP_INC) {
      inc(l);
    } else if (o == OP_DEC) {
      dec(l);
    }
  } else {
    jump(m);
    return expr_factor();
  }

  return 1;
}

void exp_expr_suf() {
  if(!expr_suf()) {
    panic_parse("expr_suf");
  }
}

int expr_unary() {
  int o = these_op(group_unary, count_unary);
  if(!expr_suf()) {
    if(o) {
      panic_parse("expr_unary");
    } else {
      return 0;
    }
  }

  if(o == OP_SUB) {
    pushn(-1);
    mul(TY_INT);
  } else if(o == OP_NOT) {
    not_(TY_INT);
  }

  return 1;
}

void exp_expr_unary() {
  if(!expr_unary()) {
    panic_parse("expr_unary");
  }
}

int expr_mul() {
  if(!expr_unary()){
    return 0;
  }

  int o;
  while((o = these_op(group_mul, count_mul))) {
    exp_expr_unary();

    if(o == OP_MUL) {
      mul(TY_INT);
    } else if (o == OP_DIV) {
      div(TY_INT);
    } else if (o == OP_REM) {
      rem(TY_INT);
    } else {
      eputs("Unknown operator");
      sys_exit(1);
    }

  }

  return 1;
}

void exp_expr_mul() {
  if(!expr_mul()) {
    panic_parse("expr_mul");
  }
}

int expr_add() {
  if(!expr_mul()) {
    return 0;
  }

  int o;
  while((o = these_op(group_add, count_add))) {
    exp_expr_mul();

    if(o == OP_ADD) {
      add(TY_INT);
    } else if (o == OP_SUB) {
      sub(TY_INT);
    } else {
      eputs("Unknown operator");
      sys_exit(1);
    }
  }

  return 1;
}

void exp_expr_add() {
  if(!expr_add()) {
    panic_parse("expr_add");
  }
}

int expr_cmp() {
  if(!expr_add()) {
    return 0;
  }

  int o;
  while((o = these_op(group_cmp, count_cmp))) {
    exp_expr_add();

    if(o == OP_LESS) {
      less(TY_INT);
    } else if(o == OP_LEQ) {
      leq(TY_INT);
    } else if(o == OP_MORE) {
      more(TY_INT);
    } else if(o == OP_MEQ) {
      meq(TY_INT);
    }
  }

  return 1;
}

void exp_expr_cmp() {
  if(!expr_cmp()) {
    panic_parse("expr_cmp");
  }
}

int expr_eq() {
  if(!expr_cmp()) {
    return 0;
  }

  int o;
  while((o = these_op(group_eq, count_eq))) {
    exp_expr_cmp();

    if(o == OP_EQ) {
      eq(TY_INT);
    } else if(o == OP_NEQ) {
      neq(TY_INT);
    }
  }

  return 1;
}

void exp_expr_eq() {
  if(!expr_eq()) {
    panic_parse("expr_eq");
  }
}

int expr_and() {
  return expr_eq();
}

void exp_expr_and() {
  if(!expr_and()) {
    panic_parse("expr_and");
  }
}

int expr_or() {
  return expr_and();
}

void exp_expr_or() {
  if(!expr_or()) {
    panic_parse("expr_or");
  }
}

void exp_expr_asg();

int expr_asg() {
  int m = mark();
  int l = lval();
  if(!l) {
    unmark(m);
    return expr_or();
  }

  int o = these_op(group_asg, count_asg);
  if(o) {
    unmark(m);
    exp_expr_asg();
    if(o == OP_ASG) {
      asg(l);
    } else if(o == OP_ADDASG) {
      addasg(l);
    } else if(o == OP_SUBASG) {
      subasg(l);
    } else if(o == OP_MULASG) {
      mulasg(l);
    } else if(o == OP_DIVASG) {
      divasg(l);
    } else if(o == OP_REMASG) {
      remasg(l);
    }
  } else {
    jump(m);
    return expr_or();
  }

  return 1;
}

void exp_expr_asg() {
  if(!expr_asg()) {
    panic_parse("expr_asg");
  }
}

int expr() {
  return expr_asg();
}

void exp_expr() {
  if(!expr()) {
    panic_parse("expr");
  }
}

int stmt() {
  if(this_str("return")) {
    exp_expr();
    ret();
  } else if(!expr()) {
    return 0;
  }

  exp_this_ch(';');
  return 1;
}

void exp_stmt() {
  if(!stmt()) {
    panic_parse("stmt");
  }
}

void program() {
  while(stmt()) {}
}

void parse() {
  func("main");
  program();
  func_fin();

  if(!at_eof()) {
    eputs("WARN: Unread characters are remain.");
  }
}
