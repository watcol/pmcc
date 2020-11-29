#include "teal.h"

int main() {
  init_lexer();
  init_code();
  parse();
  return 0;
}
