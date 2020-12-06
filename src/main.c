#include "teal.h"

int main() {
  initLexer();
  initCode();
  parse();
  return 0;
}
