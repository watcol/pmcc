#include"gen.h"

void initCode() {
  cleanVar();
  cleanGVar();
  cleanBb();
  cleanFunc();
}

void finCode() {
  declFuncs();
}
