#include"gen.h"
#define MAX_GVARS 10

#define GVAR_UNDEF 0
#define GVAR_VAR 1
#define GVAR_ARRAY 2
#define GVAR_ARRAY_ARRAY 3

int gvars_offset;
char* gvars[MAX_GVARS];
int gvars_len[MAX_GVARS];
int gvars_ty[MAX_GVARS];
int gvars_count[MAX_GVARS];
int gvars_count2[MAX_GVARS];

void cleanGVar() {
  gvars_offset = 0;

  int c = 0;
  while(c < MAX_GVARS) {
    gvars[c] = NULL;
    gvars_len[c] = 0;
    gvars_ty[c] = TY_UNKNOWN;
    gvars_count[c] = 0;
    gvars_count2[c] = 0;
    c++;
  }
}

int defGVar(char* buf, int len, int ty) {
  gvars[gvars_offset] = buf;
  gvars_len[gvars_offset] = len;
  gvars_ty[gvars_offset] = ty;
  gvars_count[gvars_offset] = 0;
  gvars_count2[gvars_offset] = 0;
  gvars_offset++;

  llDefGVar(buf, len, ty);
  return gvars_offset-1;
}

int defGArray(char* buf, int len, int ty, int num) {
  gvars[gvars_offset] = buf;
  gvars_len[gvars_offset] = len;
  gvars_ty[gvars_offset] = ty;
  gvars_count[gvars_offset] = num;
  gvars_count2[gvars_offset] = 0;
  gvars_offset++;

  llDefGArray(buf, len, ty, num);
  return gvars_offset-1;
}

int defGArray2(char* buf, int len, int ty, int num1, int num2) {
  gvars[gvars_offset] = buf;
  gvars_len[gvars_offset] = len;
  gvars_ty[gvars_offset] = ty;
  gvars_count[gvars_offset] = num1;
  gvars_count2[gvars_offset] = num2;
  gvars_offset++;

  llDefGArray2(buf, len, ty, num1, num2);
  return gvars_offset-1;

}

int gVarFind(char* buf, int len) {
  int c = 0;
  while(!(strCmp(gvars[c], buf, len) && len == gvars_len[c]) && c < gvars_offset) c++;

  if(c == gvars_offset) return -2;

  int ty = gvars_ty[c];
  int num = gvars_count[c];
  int var;
  if(num == 0) {
    var = lTmpVar(llRefTy(llRefTy(ty)));
    llAlloca(var);
    llStoreG(var, buf, len);
    var = derefVar(var);
  } else {
    var = lTmpVar(llRefTy(ty));
    llGArrayToPtr(var, buf, len, num);
    var = refVar(var);
  }

  return var;
}
