#include"parse.h"

int type() {
  int ty = TY_UNKNOWN;

  if(thisStr("void")) ty = TY_VOID;
  else if (thisStr("bool")) ty = TY_I1;
  else if (thisStr("char")) ty = TY_U8;
  else if (thisStr("int")) ty = TY_I32;
  else if (thisStr("long")) ty = TY_I64;
  else return ty;

  while(thisCh('*')) ty = llRefTy(ty);

  return ty;
}

int expType() {
  int res = type();
  if(!res) panicParse();
  return res;
}
