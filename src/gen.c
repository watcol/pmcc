#include"teal.h"

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
