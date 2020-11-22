#include"teal.h"

#define STDIN 0
#define STDOUT 1
#define STDERR 2

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

void sys_exit(int code) {
  syscall(60, code);
}

int read(void *buf, int count) {
  return syscall(0, STDIN, buf, count);
}

int ended() {
  return read(NULL, 1) == 0;
}

int write(void *buf, int count) {
  return syscall(1, STDOUT, buf, count);
}

int ewrite(void *buf, int count) {
  return syscall(1, STDERR, buf, count);
}

int putc(char c) {
  char buf[] = {c};
  return write(buf, 1);
}

int eputc(char c) {
  char buf[] = {c};
  return ewrite(buf, 1);
}

int put(char *s) {
  int len = length(s);
  return write(s, len);
}

int eput(char *s) {
  int len = length(s);
  return ewrite(s, len);
}

int puts(char *s) {
  put(s);
  putc('\n');
  return 0;
}

int eputs(char *s) {
  eput(s);
  eputc('\n');
  return 0;
}
