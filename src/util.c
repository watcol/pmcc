#include "teal.h"

void read_stdin(char *buf, int count) {
  int rc = read(buf, count);
  buf[rc] = '\0';

  if (!ended(0)) {
    eputs("The source is too long.");
    sys_exit(1);
  }
}
