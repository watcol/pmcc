#include"gen.h"

void initCode() {
  cleanVar();
  cleanBb();
  cleanFunc();
}

void finCode() {
  declFuncs();
}
