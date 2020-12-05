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

int exp_expr();

int expr_factor() {
  if(this_ch('(')) {
    int ty = exp_expr();
    exp_this_ch(')');
    return ty;
  } else if(lex_num()) {
    pushl();
    return TY_I32;
  } else {
    int l = lval();
    if (l) {
      pushm(l);
      return get_type(l);
    } else {
      return TY_UNKNOWN;
    }
  }
}

int exp_expr_factor() {
  int res = expr_factor();
  if(!res) {
    panic_parse("expr_factor");
  }
  return res;
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

  return get_type(l);
}

int exp_expr_suf() {
  int res = expr_suf();
  if(!res) {
    panic_parse("expr_suf");
  }
  return res;
}

int expr_unary() {
  int o = these_op(group_unary, count_unary);
  int ty = expr_suf();
  if(!ty) {
    if(o) {
      panic_parse("expr_unary");
    } else {
      return TY_UNKNOWN;
    }
  }

  if(o == OP_SUB) {
    pushn(-1);
    mul(ty);
  } else if(o == OP_NOT) {
    not_(ty);
  }

  return ty;
}

int exp_expr_unary() {
  int res = expr_unary();
  if(!res) {
    panic_parse("expr_unary");
  }
  return res;
}

int expr_mul() {
  int ty1 = expr_unary();
  if(!ty1){
    return 0;
  }

  int o;
  while((o = these_op(group_mul, count_mul))) {
    int ty2 = exp_expr_unary();
    if(ty1 != ty2) {
      panic_parse("type error");
    }

    if(o == OP_MUL) {
      mul(ty1);
    } else if (o == OP_DIV) {
      div(ty1);
    } else if (o == OP_REM) {
      rem(ty1);
    } else {
      panic("Unknown operator");
    }

  }

  return ty1;
}

int exp_expr_mul() {
  int res = expr_mul();
  if(!res) {
    panic_parse("expr_mul");
  }
  return res;
}

int expr_add() {
  int ty1 = expr_mul();
  if(!ty1) {
    return TY_UNKNOWN;
  }

  int o;
  while((o = these_op(group_add, count_add))) {
    int ty2 = exp_expr_mul();
    if(ty1 != ty2) {
      panic("Type unmatched");
    }

    if(o == OP_ADD) {
      add(ty1);
    } else if (o == OP_SUB) {
      sub(ty1);
    } else {
      panic("Unknown operator");
    }
  }

  return ty1;
}

int exp_expr_add() {
  int res = expr_add();
  if(!res) {
    panic_parse("expr_add");
  }
  return res;
}

int expr_cmp() {
  int ty1 = expr_add();
  if(!ty1) {
    return TY_UNKNOWN;
  }

  int o;
  while((o = these_op(group_cmp, count_cmp))) {
    int ty2 = exp_expr_add();
    if(ty1 != ty2) {
      panic("Type unmatched");
    }

    if(o == OP_LESS) {
      less(ty1);
    } else if(o == OP_LEQ) {
      leq(ty1);
    } else if(o == OP_MORE) {
      more(ty1);
    } else if(o == OP_MEQ) {
      meq(ty1);
    }
  }

  return ty1;
}

int exp_expr_cmp() {
  int res = expr_cmp();
  if(!res) {
    panic_parse("expr_cmp");
  }
  return res;
}

int expr_eq() {
  int ty1 = expr_cmp();
  if(!ty1) {
    return TY_UNKNOWN;
  }

  int o;
  while((o = these_op(group_eq, count_eq))) {
    int ty2 = exp_expr_cmp();
    if(ty1 != ty2) {
      panic("Type unmatched");
    }

    if(o == OP_EQ) {
      eq(ty1);
    } else if(o == OP_NEQ) {
      neq(ty1);
    }
  }

  return ty1;
}

int exp_expr_eq() {
  int res = expr_eq();
  if(!res) {
    panic_parse("expr_eq");
  }
  return res;
}

int expr_and() {
  int ty1 = expr_eq();
  if(!ty1) {
    return TY_UNKNOWN;
  }

  int o;
  while((o = these_op(group_and, count_and))) {
    int ty2 = exp_expr_eq();
    if(ty1 != ty2) {
      panic("Type unmatched");
    }

    if(o == OP_AND) and_();
  }

  return ty1;
}

int exp_expr_and() {
  int res = expr_and();
  if(!res) {
    panic_parse("expr_and");
  }
  return res;
}

int expr_or() {
  int ty1 = expr_and();
  if(!ty1) {
    return TY_UNKNOWN;
  }

  int o;
  while((o = these_op(group_or, count_or))) {
    int ty2 = exp_expr_and();
    if(ty1 != ty2) {
      panic("Type unmatched");
    }

    if(o == OP_OR) or_();
  }

  return ty1;
}

int exp_expr_or() {
  int res = expr_or();
  if(!res) {
    panic_parse("expr_or");
  }
  return res;
}

int exp_expr_asg();

int expr_asg() {
  int m = mark();
  int l = lval();
  int ty = get_type(l);
  if(!l) {
    unmark(m);
    return expr_or();
  }

  int o = these_op(group_asg, count_asg);
  if(o) {
    unmark(m);
    if(ty != exp_expr_asg()) {
      panic("Type unmatched");
    }

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

  return ty;
}

int exp_expr_asg() {
  int res = expr_asg();
  if(!res) {
    panic_parse("expr_asg");
  }
  return res;
}

int expr() {
  return expr_asg();
}

int exp_expr() {
  int res = expr();
  if(!res) {
    panic_parse("expr");
  }
  return res;
}

int stmt();
void exp_stmt();

int stmt_single() {
  if(!expr()) return 0;

  exp_this_ch(';');
  return 1;
}

int stmt_multi() {
  if(!this_ch('{')) return 0;

  while(stmt()) {}
  exp_this_ch('}');
  return 1;
}

int stmt_ret() {
  if(!this_str("return")) return 0;

  exp_expr();
  exp_this_ch(';');
  ret();
  return 1;
}

int stmt_if() {
  if(!this_str("if")) return 0;

  exp_this_ch('(');
  exp_expr();
  exp_this_ch(')');

  int id = if_begin();
  exp_stmt();

  if_else(id);
  if(this_str("else")) exp_stmt();

  if_end(id);

  return 1;
}

int stmt_while() {
  if(!this_str("while")) return 0;

  int id = while_begin();

  exp_this_ch('(');
  exp_expr();
  exp_this_ch(')');
  while_eval(id);

  exp_stmt();
  while_end(id);

  return 1;
}

int stmt() {
  if(!stmt_multi() && !stmt_ret() && !stmt_if() && !stmt_while()) {
    return stmt_single();
  }

  return 1;
}


void exp_stmt() {
  if(!stmt()) {
    panic_parse("stmt");
  }
}

void program() {
  stmt();
}

void parse() {
  func_begin("main", TY_I32, NULL, 0);
  //program();
  func_end();

  if(!at_eof()) {
    eputs("WARN: Unread characters are remain.");
  }
}
