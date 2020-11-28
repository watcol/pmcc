#include"teal.h"

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
  } else if (i == VAL_AL) {
    put("al");
  } else if (i < 0) {
    put("[rbp-");
    putnum(-(i+1));
    putc(']');
  } else {
    eputs("Unknown operand type.");
    sys_exit(1);
  }
}

void init_code() {
  puts(".intel_syntax noprefix");
}

void func(char* name) {
  put(".globl ");
  puts(name);

  put(name);
  puts(":");
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
