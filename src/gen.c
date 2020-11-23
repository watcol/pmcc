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

void ret() {
  puts("  ret");
}
