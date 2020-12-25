#include"parse.h"

int func() {
  int ty = type();
  if(!ty) return 0;
  char* name = getCursor();
  int len = expIdent();
  if(thisCh(';')) {
    defGVar(name, len, ty);
    return 1;
  }
  expThisCh('(');

  char* args[MAX_ARGS];
  int arg_lens[MAX_ARGS];
  int arg_tys[MAX_ARGS];
  int c = 0;

  arg_tys[c] = type();
  if(arg_tys[c]) {
    args[c] = getCursor();
    arg_lens[c] = expIdent();
    c++;
  }

  while(thisCh(',')) {
    arg_tys[c] = expType();
    args[c] = getCursor();
    arg_lens[c] = expIdent();
    c++;
  }
  expThisCh(')');

  if(thisCh(';')) {
    funcDecl(name, len, ty, arg_tys, c);
    return 1;
  }

  funcBegin(name, len, ty, args, arg_lens, arg_tys, c);
  expStmt();
  funcEnd();
  return 1;
}
