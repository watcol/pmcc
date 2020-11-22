#include "teal.h"
#include <stdio.h>

void read_stdin(char *buf, int count) {
  int rc = read(0, buf, count);
  buf[rc] = '\0';

  if (getc(stdin) != EOF) {
    fprintf(stderr, "The source is too long.");
    sys_exit(1);
  }
}
