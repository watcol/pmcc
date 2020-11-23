#include"teal.h"

void putval(int i) {
  if (i == REG_RAX) {
    put("rax");
  } else if (i == REG_RDI) {
    put("rdi");
  } else {
    putlen(i);
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

void inst1(char* in, int val) {
  put("  ");
  put(in);
  putc(' ');
  putval(val);
  putc('\n');
}

void inst2(char* in, int dst, int src) {
  put("  ");
  put(in);
  putc(' ');
  putval(dst);
  put(", ");
  putval(src);
  putc('\n');
}

void ret() {
  puts("  ret");
}
