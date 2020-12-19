#include"teal.h"

int func() {
  char* name = getCursor();
  int len = lexIdent();
  if(!len) return 0;
  expThisCh('(');

  char* args[MAX_ARGS];
  int arg_lens[MAX_ARGS];
  int arg_tys[MAX_ARGS];
  int c = 0;
  args[c] = getCursor();
  arg_lens[c] = lexIdent();
  arg_tys[c] = TY_I32;
  if(arg_lens[c]) c++;
  while(thisCh(',')) {
    args[c] = getCursor();
    arg_lens[c] = expIdent();
    arg_tys[c] = TY_I32;
    c++;
  }
  expThisCh(')');

  if(thisCh(';')) {
    funcDecl(name, len, TY_I32, arg_tys, c);
    return 1;
  }

  funcBegin(name, len, TY_I32, args, arg_lens, arg_tys, c);
  expStmt();
  funcEnd();
  return 1;
}

