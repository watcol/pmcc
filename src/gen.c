#include"teal.h"

void putval(int i) {
  if (i == REG_LEX) {
    lex_put();
  } else if (i == REG_RAX) {
    put("rax");
  } else if (i == REG_RDI) {
    put("rdi");
  } else if (i == REG_RSI) {
    put("rsi");
  } else if (i == REG_RDX) {
    put("rdx");
  } else if (i == REG_RCX) {
    put("rcx");
  } else if (i == REG_R8) {
    put("r8");
  } else if (i == REG_R9) {
    put("r9");
  } else if (i == REG_AL) {
    put("al");
  } else {
    eputs("Unknown register type.");
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

void instr(char* in, int val) {
  put("  ");
  put(in);
  putc(' ');
  putval(val);
  putc('\n');
}

void instv(char* in, char* val) {
  put("  ");
  put(in);
  putc(' ');
  put(val);
  putc('\n');
}

void instrr(char* in, int dst, int src) {
  put("  ");
  put(in);
  putc(' ');
  putval(dst);
  put(", ");
  putval(src);
  putc('\n');
}

void instrv(char* in, int dst, char* src) {
  put("  ");
  put(in);
  putc(' ');
  putval(dst);
  put(", ");
  put(src);
  putc('\n');
}
