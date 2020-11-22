#include "teal.h"
#include <stdio.h>
#include <stdlib.h>

void read_stdin(char *buf, int count) {
  int rc = fread(buf, 1, count, stdin);
  buf[rc] = '\0';

  if (getc(stdin) != EOF) {
    fprintf(stderr, "The source is too long.");
    exit(1);
  }
}
