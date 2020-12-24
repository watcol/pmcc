#include"parse.h"

int type() {
  int ty = TY_UNKNOWN;

  if(thisIdent("void")) ty = TY_VOID;
  else if (thisIdent("bool")) ty = TY_I1;
  else if (thisIdent("char")) ty = TY_U8;
  else if (thisIdent("int")) ty = TY_I32;
  else if (thisIdent("long")) ty = TY_I64;
  else return ty;

  while(thisCh('*')) ty = llRefTy(ty);

  return ty;
}

int expType() {
  int res = type();
  if(!res) panicParse("type");
  return res;
}
