#include"teal.h"

#define MAX_VARS 200

int cur_num;
char* vars[MAX_VARS];
int cur_offset;
int offsets[MAX_VARS];
int types[MAX_VARS];

void init_code() {
  cur_num = 0;
  cur_offset = 0;
  int c = 0;
  while(c < MAX_VARS) {
    vars[c] = NULL;
    offsets[c] = 0;
    types[c] = 0;
    c++;
  }

  puts(".intel_syntax noprefix");
}


int get_varid(char* str, int len, int type) {
  int c = 0;
  while(!str_cmp(str, vars[c], len) && c != cur_num) {
    c++;
  }

  if(c == cur_num) {
    if(type == TY_UNKNOWN) {
      eputs("Type must be known in the declaration.");
      sys_exit(1);
    }

    cur_num++;
    vars[c] = str;
    cur_offset+=type;
    offsets[c] = c*type;
    types[c] = type;

    if(cur_offset >= MAX_VARS) {
      eputs("Too many variables");
      sys_exit(1);
    }
  }

  return -(c+1);
}


void putval(int i) {
  if (i == VAL_LEX) {
    lex_put();
  } else if (i == VAL_RAX) {
    put("rax");
  } else if (i == VAL_RDI) {
    put("rdi");
  } else if (i == VAL_RSI) {
    put("rsi");
  } else if (i == VAL_RDX) {
    put("rdx");
  } else if (i == VAL_RCX) {
    put("rcx");
  } else if (i == VAL_R8) {
    put("r8");
  } else if (i == VAL_R9) {
    put("r9");
  } else if (i == VAL_RSP) {
    put("rsp");
  } else if (i == VAL_RBP) {
    put("rbp");
  } else if (i == VAL_AL) {
    put("al");
  } else if (i < 0) {
    int id = -(i+1);
    if(types[id] == TY_CHAR) {
      put("BYTE");
    } else if (types[id] == TY_SHORT) {
      put("WORD");
    } else if (types[id] == TY_INT) {
      put("DWORD");
    } else if (types[id] == TY_LONG) {
      put("QWORD");
    }

    put(" ptr [rbp-");
    putnum(offsets[id]);
    putc(']');
  } else {
    eputs("Unknown operand type.");
    sys_exit(1);
  }
}

void func(char* name) {
  put(".globl ");
  puts(name);

  put(name);
  puts(":");

  // Prologue
  instv("push", VAL_RBP);
  instvv("mov", VAL_RBP, VAL_RSP);
  instvs("sub", VAL_RSP, "200"); // MAX_VARS
}

void func_fin() {
  // Epilogue
  instvv("mov", VAL_RSP, VAL_RBP);
  instv("pop", VAL_RBP);
  inst("ret");
}

void inst(char* in) {
  put("  ");
  put(in);
  putc('\n');
}

void instv(char* in, int val) {
  put("  ");
  put(in);
  putc(' ');
  putval(val);
  putc('\n');
}

void insts(char* in, char* val) {
  put("  ");
  put(in);
  putc(' ');
  put(val);
  putc('\n');
}

void instvv(char* in, int dst, int src) {
  put("  ");
  put(in);
  putc(' ');
  putval(dst);
  put(", ");
  putval(src);
  putc('\n');
}

void instvs(char* in, int dst, char* src) {
  put("  ");
  put(in);
  putc(' ');
  putval(dst);
  put(", ");
  put(src);
  putc('\n');
}
