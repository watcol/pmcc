#include<stdio.h> // debug
#include"teal.h"

#define MAX_INPUT 50000

int main() {
  // Read STDIN.
  char buf[MAX_INPUT+1] = "";
  read_stdin(buf, MAX_INPUT);

  // debug: Print the buffer.
  fprintf(stderr, "%s", buf);
  return 0;
}
