#include "teal.h"

#define MAX_INT 20

int isSpace(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' ||
         c == '\r';
}

int isDigit(char c) { return c >= '0' && c <= '9'; }

int isAlpha(char c) {
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int isAlphaNum(char c) { return isDigit(c) || isAlpha(c); }

int length(char *buf) {
  int len = 0;
  while (*(buf++)) {
    len++;
  }

  return len;
}

int strCmp(char *buf1, char *buf2, int len) {
  int c = 0;
  if (buf1 == NULL || buf2 == NULL) {
    return 0;
  }

  while (c < len) {
    if (buf1[c] != buf2[c]) {
      return 0;
    }
    c++;
  }
  return 1;
}

int strToI(char *buf, char **ret) {
  int i = 0;

  if(!isDigit(*buf)) return -1;

  while(isDigit(*buf)) {
    i *= 10;
    i += *buf - '0';
    buf++;
  }

  *ret = buf;
  return i;
}

int digitLen(char *buf) {
  int len = 0;
  while (isDigit(*(buf++))) {
    len++;
  }
  return len;
}

int identLen(char *buf) {
  if (!isAlpha(*buf)) {
    return 0;
  }

  int len = 1;
  buf++;
  while (isAlphaNum(*buf) || *buf == '_') {
    len++;
    buf++;
  }

  return len;
}

int ended() { return read(NULL, 1) == 0; }

void putC(char c) {
  char buf[] = {c};
  write(buf, 1);
}

void ePutC(char c) {
  char buf[] = {c};
  eWrite(buf, 1);
}

void put(char *s) {
  int len = length(s);
  write(s, len);
}

void ePut(char *s) {
  int len = length(s);
  eWrite(s, len);
}

void putS(char *s) {
  put(s);
  putC('\n');
}

void ePutS(char *s) {
  ePut(s);
  ePutC('\n');
}

void panic(char* msg) {
  ePutS(msg);
  sysExit(1);
}

void putNum(int n) {
  char buf[MAX_INT + 1];
  int c = MAX_INT;

  if (n < 0) {
    putC('-');
    n *= -1;
  }

  if (n == 0) {
    buf[c] = '0';
    c--;
  }

  while (n != 0) {
    buf[c] = '0' + (n % 10);
    n /= 10;
    c--;
  }

  put(&buf[c + 1]);
}

void ePutNum(int n) {
  char buf[MAX_INT + 1];
  int c = MAX_INT;

  if (n < 0) {
    ePutC('-');
    n *= -1;
  }

  if (n == 0) {
    buf[c] = '0';
    c--;
  }

  while (n != 0) {
    buf[c] = '0' + (n % 10);
    n /= 10;
    c--;
  }

  ePut(&buf[c + 1]);
}

void readStdin(char *buf, int count) {
  int rc = read(buf, count);
  buf[rc] = '\0';

  if (!ended()) {
    panic("The source is too long.");
  }
}
