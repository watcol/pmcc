#include"teal.h"

int is_space(char c) {
  return c == ' '  || c == '\t' ||
         c == '\n' || c == '\v' ||
         c == '\f' || c == '\r';
}

int is_digit(char c) {
  return c == '0' || c == '1' ||
         c == '2' || c == '3' ||
         c == '4' || c == '5' ||
         c == '6' || c == '7' ||
         c == '8' || c == '9';
}
