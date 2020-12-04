#include"teal.h"
#define MAX_VARS 200

int cur_num;
char* vars[MAX_VARS];
int cur_offset;
int offsets[MAX_VARS];
int types[MAX_VARS];
int lb_id;

void init_code() {
  cur_num = 0;
  cur_offset = 0;
  int c = 0;
  while(c < MAX_VARS) {
    vars[c] = NULL;
    offsets[c] = 0;
    types[c] = TY_UNKNOWN;
    c++;
  }

  lb_id = 0;
  puts(".intel_syntax noprefix");
}


int get_varid(char* str, int len, int type) {
  int c = 0;
  while(!str_cmp(str, vars[c], len) && c != cur_num) {
    c++;
  }

  if(c == cur_num) {
    if(type == TY_UNKNOWN) {
      panic("Type must be known in the declaration.");
    }

    cur_num++;
    vars[c] = str;
    cur_offset+=type;
    offsets[c] = c*type;
    types[c] = type;

    if(cur_offset >= MAX_VARS) {
      panic("Too many variables");
    }
  }

  return c + 1;
}

int get_type(int id) {
  return types[id - 1];
}

int get_offset(int id) {
  return offsets[id - 1];
}

void func(char* name) {
  put(".globl ");
  puts(name);

  put(name);
  puts(":");

  // Prologue
  instr("push", REG_RBP);
  instrr("mov", REG_RBP, REG_RSP);
  instrn("sub", REG_RSP, MAX_VARS);
}

void func_fin() {
  // Epilogue
  instrr("mov", REG_RSP, REG_RBP);
  instr("pop", REG_RBP);
  inst("ret");
}

void extend(int reg, int type) {
  int big = conv_type(reg, TY_LONG);
  int orig = conv_type(reg, type);

  if(type != TY_LONG && type != TY_INT) {
    instrr("movzx", big, orig);
  }
}

void pushl() {
  instl("push");
}

void pushm(int val) {
  int type = get_type(val);
  int rax = conv_type(REG_RAX, type);

  if (type == TY_LONG) {
    instm("push", val);
  } else {
    instrm("mov", rax, val);
    extend(REG_RAX, type);
    instr("push", REG_RAX);
  }
}

void pushn(int val) {
  instn("push", val);
}

void inc(int mem) {
  int type = get_type(mem);
  int rax = conv_type(REG_RAX, type);
  instrm("mov", rax, mem);
  instm("inc", mem);
  extend(REG_RAX, type);
  instr("push", REG_RAX);
}

void dec(int mem) {
  int type = get_type(mem);
  int rax = conv_type(REG_RAX, type);
  instrm("mov", rax, mem);
  instm("dec", mem);
  extend(REG_RAX, type);
  instr("push", REG_RAX);
}

void add(int type) {
  int rax = conv_type(REG_RAX, type);
  int rdi = conv_type(REG_RDI, type);
  instr("pop", REG_RDI);
  instr("pop", REG_RAX);
  instrr("add", rax, rdi);
  extend(REG_RAX, type);
  instr("push", REG_RAX);
}

void sub(int type) {
  int rax = conv_type(REG_RAX, type);
  int rdi = conv_type(REG_RDI, type);
  instr("pop", REG_RDI);
  instr("pop", REG_RAX);
  instrr("sub", rax, rdi);
  extend(REG_RAX, type);
  instr("push", REG_RAX);
}

void mul(int type) {
  int rax = conv_type(REG_RAX, type);
  int rdi = conv_type(REG_RDI, type);
  instr("pop", REG_RDI);
  instr("pop", REG_RAX);
  instrr("imul", rax, rdi);
  extend(REG_RAX, type);
  instr("push", REG_RAX);
}

void div(int type) {
  int rdi = conv_type(REG_RDI, type);
  instr("pop", REG_RDI);
  instr("pop", REG_RAX);

  if(type == TY_CHAR) {
    panic("Division for char is not supported.");
  } else if(type == TY_SHORT) {
    inst("cwd");
  } else if(type == TY_INT) {
    inst("cdq");
  } else if(type == TY_LONG) {
    inst("cqo");
  }

  instr("idiv", rdi);
  extend(REG_RAX, type);
  instr("push", REG_RAX);
}

void rem(int type) {
  int rdi = conv_type(REG_RDI, type);
  instr("pop", REG_RDI);
  instr("pop", REG_RAX);

  if(type == TY_CHAR) {
    panic("Division for char is not supported.");
  } else if(type == TY_SHORT) {
    inst("cwd");
  } else if(type == TY_INT) {
    inst("cdq");
  } else if(type == TY_LONG) {
    inst("cqo");
  }

  instr("idiv", rdi);
  extend(REG_RDX, type);
  instr("push", REG_RDX);
}

void not_(int type) {
  int rax = conv_type(REG_RAX, type);
  instr("pop", REG_RAX);
  instrn("cmp", rax, 0);
  instr("sete", REG_AL);
  instrr("movzb", REG_RAX, REG_AL);
  instr("push", REG_RAX);
}

void less(int type) {
  int rax = conv_type(REG_RAX, type);
  int rdi = conv_type(REG_RDI, type);
  instr("pop", REG_RDI);
  instr("pop", REG_RAX);
  instrr("cmp", rax, rdi);
  instr("setl", REG_AL);
  instrr("movzb", REG_RAX, REG_AL);
  instr("push", REG_RAX);
}

void leq(int type) {
  int rax = conv_type(REG_RAX, type);
  int rdi = conv_type(REG_RDI, type);
  instr("pop", REG_RDI);
  instr("pop", REG_RAX);
  instrr("cmp", rax, rdi);
  instr("setle", REG_AL);
  instrr("movzb", REG_RAX, REG_AL);
  instr("push", REG_RAX);
}

void more(int type) {
  int rax = conv_type(REG_RAX, type);
  int rdi = conv_type(REG_RDI, type);
  instr("pop", REG_RDI);
  instr("pop", REG_RAX);
  instrr("cmp", rdi, rax);
  instr("setl", REG_AL);
  instrr("movzb", REG_RAX, REG_AL);
  instr("push", REG_RAX);
}

void meq(int type) {
  int rax = conv_type(REG_RAX, type);
  int rdi = conv_type(REG_RDI, type);
  instr("pop", REG_RDI);
  instr("pop", REG_RAX);
  instrr("cmp", rdi, rax);
  instr("setle", REG_AL);
  instrr("movzb", REG_RAX, REG_AL);
  instr("push", REG_RAX);
}

void eq(int type) {
  int rax = conv_type(REG_RAX, type);
  int rdi = conv_type(REG_RDI, type);
  instr("pop", REG_RDI);
  instr("pop", REG_RAX);
  instrr("cmp", rax, rdi);
  instr("sete", REG_AL);
  instrr("movzb", REG_RAX, REG_AL);
  instr("push", REG_RAX);
}

void neq(int type) {
  int rax = conv_type(REG_RAX, type);
  int rdi = conv_type(REG_RDI, type);
  instr("pop", REG_RDI);
  instr("pop", REG_RAX);
  instrr("cmp", rax, rdi);
  instr("setne", REG_AL);
  instrr("movzb", REG_RAX, REG_AL);
  instr("push", REG_RAX);
}

void and_() {
  instr("pop", REG_RDI);
  int id = if_begin();
  instr("push", REG_RDI);
  if_else(id);
  instn("push", 0);
  if_end(id);
}

void or_() {
  instr("pop", REG_RDI);
  int id = if_begin();
  instr("push", REG_RAX);
  if_else(id);
  instn("push", REG_RDI);
  if_end(id);
}

void asg(int mem) {
  int rax = conv_type(REG_RAX, get_type(mem));
  instr("pop", REG_RAX);
  instmr("mov", mem, rax);
  instr("push", REG_RAX);
}

void addasg(int mem) {
  int type = get_type(mem);
  instr("pop", REG_RAX);
  pushm(mem);
  instr("push", REG_RAX);
  add(type);
  asg(mem);
}

void subasg(int mem) {
  int type = get_type(mem);
  instr("pop", REG_RAX);
  pushm(mem);
  instr("push", REG_RAX);
  sub(type);
  asg(mem);
}

void mulasg(int mem) {
  int type = get_type(mem);
  instr("pop", REG_RAX);
  pushm(mem);
  instr("push", REG_RAX);
  mul(type);
  asg(mem);
}

void divasg(int mem) {
  int type = get_type(mem);
  instr("pop", REG_RAX);
  pushm(mem);
  instr("push", REG_RAX);
  div(type);
  asg(mem);
}

void remasg(int mem) {
  int type = get_type(mem);
  instr("pop", REG_RAX);
  pushm(mem);
  instr("push", REG_RAX);
  rem(type);
  asg(mem);
}

void ret() {
  instr("pop", REG_RAX);
  func_fin();
}

int if_begin() {
  lb_id++;
  instr("pop", REG_RAX);
  instrn("cmp", REG_RAX, 0);
  instlb("je", lb_id, LB_ELSE);
  return lb_id;
}

void if_else(int id) {
  instlb("jmp", id, LB_END);
  label(id, LB_ELSE);
}

void if_end(int id) {
  label(id, LB_END);
}
