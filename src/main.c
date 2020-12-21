#include "teal.h"

int main() {
  initLexer();
  initCode();
  parse();
  finCode();
  return 0;
}
