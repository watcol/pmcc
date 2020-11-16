#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_INPUT 50000

int main() {
  // Read STDIN.
  char buf[MAX_INPUT+1] = "";
  size_t rc = fread(buf, 1, MAX_INPUT, stdin);
  buf[rc] = '\0';

  // Check whether the file reached EOF.
  if(getc(stdin) != EOF) {
    printf("The source is too long.\n");
    exit(1);
  }

  // debug: Print the buffer.
  printf("%s\n", buf);
  return 0;
}
