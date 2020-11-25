#include "teal.h"

int is_space(char c) {
  return c == ' '  || c == '\t' ||
         c == '\n' || c == '\v' ||
         c == '\f' || c == '\r';
}

int is_digit(char c) {
  return c >= '0' && c <= '9';
}

int length(char* buf) {
  int len = 0;
  while(*(buf++)) {
    len++;
  }

  return len;
}

int digitlen(char* buf) {
  int len = 0;
  while(is_digit(*(buf++))) {
    len++;
  }
  return len;
}

int ended() {
  return read(NULL, 1) == 0;
}

void putc(char c) {
  char buf[] = {c};
  write(buf, 1);
}

void eputc(char c) {
  char buf[] = {c};
  ewrite(buf, 1);
}

void put(char *s) {
  int len = length(s);
  write(s, len);
}

void eput(char *s) {
  int len = length(s);
  ewrite(s, len);
}

void puts(char *s) {
  put(s);
  putc('\n');
}

void eputs(char *s) {
  eput(s);
  eputc('\n');
}

void read_stdin(char *buf, int count) {
  int rc = read(buf, count);
  buf[rc] = '\0';

  if (!ended()) {
    eputs("The source is too long.");
    sys_exit(1);
  }
}
