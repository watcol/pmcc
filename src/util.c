#include "teal.h"

#define MAX_INT 20

int is_space(char c) {
  return c == ' '  || c == '\t' ||
         c == '\n' || c == '\v' ||
         c == '\f' || c == '\r';
}

int is_digit(char c) {
  return c >= '0' && c <= '9';
}

int is_alpha(char c) {
  return (c >= 'A' && c <= 'Z') ||
         (c >= 'a' && c <= 'z');
}

int is_alphanum(char c) {
  return is_digit(c) || is_alpha(c);
}

int length(char* buf) {
  int len = 0;
  while(*(buf++)) {
    len++;
  }

  return len;
}

int str_cmp(char* buf1, char* buf2, int len) {
  int c = 0;
  if (buf1 == NULL || buf2 == NULL) {
    return 0;
  }

  while(c < len) {
    if(buf1[c] != buf2[c]) {
      return 0;
    }
    c++;
  }
  return 1;
}

int digitlen(char* buf) {
  int len = 0;
  while(is_digit(*(buf++))) {
    len++;
  }
  return len;
}

int identlen(char* buf) {
  if(!is_alpha(*buf)) {
    return 0;
  }

  int len = 1;
  buf++;
  while(is_alphanum(*buf) || *buf == '_') {
    len++;
    buf++;
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

void putnum(int n) {
  char buf[MAX_INT + 1];
  int c = MAX_INT;

  if(n < 0) {
    putc('-');
    n *= -1;
  }

  if(n == 0) {
    buf[c] = '0';
    c--;
  }

  while(n != 0) {
    buf[c] = '0' + (n % 10);
    n /= 10;
    c--;
  }

  put(&buf[c+1]);
}


void eputnum(int n) {
  char buf[MAX_INT + 1];
  int c = MAX_INT;

  if(n < 0) {
    eputc('-');
    n *= -1;
  }

  if(n == 0) {
    buf[c] = '0';
    c--;
  }

  while(n != 0) {
    buf[c] = '0' + (n % 10);
    n /= 10;
    c--;
  }

  eput(&buf[c+1]);
}

void read_stdin(char *buf, int count) {
  int rc = read(buf, count);
  buf[rc] = '\0';

  if (!ended()) {
    eputs("The source is too long.");
    sys_exit(1);
  }
}
