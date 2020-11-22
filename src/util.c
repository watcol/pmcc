#include "teal.h"
#include <stdio.h>

void read_stdin(char *buf, int count) {
  int rc = read(buf, count);
  buf[rc] = '\0';

  if (!ended(0)) {
    fprintf(stderr, "The source is too long.");
    sys_exit(1);
  }
}
