#include<stdio.h> // debug
#include"teal.h"

#define MAX_INPUT 50000

int main() {
  // Read STDIN.
  char buf[MAX_INPUT+1] = "";
  read_stdin(buf, MAX_INPUT);

  // debug: Print the buffer.
  fprintf(stderr, "%s", buf);

  // Write assembly.
  puts(".intel_syntax noprefix");
  puts(".globl main");
  puts("main:");
  puts("  mov rax, 0");
  puts("  ret");

  return 0;
}
