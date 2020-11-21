#include "teal.h"
#include <stdio.h>
#include <stdlib.h>

void read_stdin(char *buf, size_t count) {
  size_t rc = fread(buf, 1, count, stdin);
  buf[rc] = '\0';

  if (getc(stdin) != EOF) {
    printf("The source is too long.");
    exit(1);
  }
}
