#include<stdio.h> // debug
#include<stdlib.h>
#include"teal.h"

#define MAX_INPUT 50000

int main() {
  // Read STDIN.
  char buf[MAX_INPUT+1] = "";
  read_stdin(buf, MAX_INPUT);

  // debug: Print the buffer.
  //fprintf(stderr, "%s", buf);

  // Read a number.
  int n = strtol(buf, NULL, 10);
  // debug: Print a receved number.
  //fprintf(stderr, "%d\n", n);

  // Write assembly.
  puts(".intel_syntax noprefix");
  puts(".globl main");
  puts("main:");
  printf("  mov rax, %d\n", n);
  puts("  ret");

  return 0;
}
