#include"gen.h"

#define MAX_FUNCS 100

#define FUNC_UNDEF 0
#define FUNC_DECL 1
#define FUNC_NODECL 2
#define FUNC_ONHOLD 3

int funcs_offset;
char* funcs_name[MAX_FUNCS];
int funcs_len[MAX_FUNCS];
int funcs_ret[MAX_FUNCS];
int funcs_args[MAX_FUNCS][MAX_ARGS];
int funcs_argc[MAX_FUNCS];
int funcs_decl[MAX_FUNCS];

int cur_func;

void cleanFunc() {
  funcs_offset = 0;
  int c = 0;
  while(c < MAX_FUNCS) {
    funcs_name[c] = NULL;
    funcs_len[c] = 0;
    funcs_ret[c] = TY_UNKNOWN;
    int d = 0;
    while(d < MAX_ARGS) {
      funcs_args[c][d] = TY_UNKNOWN;
      d++;
    }
    funcs_argc[c] = 0;
    funcs_decl[c] = FUNC_UNDEF;
    c++;
  }
}

int retTy() {
  return funcs_ret[cur_func];
}

int funcFind(char* cur, int len) {
  int c = 0;
  while(!(strCmp(funcs_name[c], cur, len) && len == funcs_len[c]) && c < funcs_offset) c++;

  if(c == funcs_offset) {
    return -1;
  }

  return c;
}

int funcAdd(char* name, int len, int ret, int* args, int argc) {
  int id = funcFind(name, len);
  if(id >= 0) {
    if(ret != funcs_ret[id]) {
      panic("Conflict function definition.");
      return -1;
    }

    if(argc != funcs_argc[id]) {
      panic("Conflict function definition.");
      return -1;
    }

    int c = 0;
    while(c < argc) {
      if(args[c] != funcs_args[id][c]) {
        panic("Conflict function definition.");
        return -1;
      }
      c++;
    }

    return id;
  }

  funcs_name[funcs_offset] = name;
  funcs_len[funcs_offset] = len;
  funcs_ret[funcs_offset] = ret;
  int c = 0;
  while(c < argc) {
    funcs_args[funcs_offset][c] = args[c];
    c++;
  }
  funcs_argc[funcs_offset] = argc;
  funcs_offset++;
  return funcs_offset-1;
}

void funcDecl(char* name, int len, int ret, int* args, int argc) {
  int id = funcAdd(name, len, ret, args, argc);
  if(funcs_decl[id] == FUNC_UNDEF) funcs_decl[id] = FUNC_ONHOLD;
}

void funcBegin(char* name, int len, int ret, char** args, int* args_len, int* args_ty, int argc) {
  int id = funcAdd(name, len, ret, args_ty, argc);
  cur_func = id;
  funcs_decl[id] = FUNC_NODECL;
  llFuncBegin(name, len, ret, args_ty, argc);

  addArgs(args, args_len, args_ty, argc);
}

void funcEnd() {
  // dummy ret
  llRetN(retTy(), 1);
  cur_func = 0;
  cleanVar();
  cleanBb();
  llFuncEnd();
}

void declFuncs() {
  int c = 0;
  while(c < funcs_offset) {
    if(funcs_decl[c] == FUNC_DECL) {
      llFuncDecl(funcs_name[c], funcs_len[c], funcs_ret[c], funcs_args[c], funcs_argc[c]);
    }
    c++;
  }
}

int funcCall(char* buf, int len, int* args, int argc) {
  int id = funcFind(buf, len);
  if(id < 0) {
    panic("Function not found.");
    return -2;
  }
  if(funcs_decl[id] == FUNC_ONHOLD) funcs_decl[id] = FUNC_DECL;

  int c = 0;
  while(c < argc) {
    args[c] = castVar(args[c], funcs_args[id][c]);
    args[c] = derefVar(args[c]);
    c++;
  }
  int dst = VAR_VOID;
  int ret_ty = funcs_ret[id];
  if(ret_ty == TY_VOID) {
    llFuncCall(buf, len, ret_ty, args, argc);
  } else {
    dst = lTmpVar(ret_ty);
    llFuncCallAsg(buf, len, dst, args, argc);
    dst = refVar(dst);
  }
  return dst;
}

