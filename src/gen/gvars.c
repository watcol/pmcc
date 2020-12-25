#include"gen.h"
#define MAX_GVARS 10

int gvars_offset;
char* gvars[MAX_GVARS];
int gvars_len[MAX_GVARS];
int gvars_ty[MAX_GVARS];

void cleanGVar() {
  gvars_offset = 0;

  int c = 0;
  while(c < MAX_GVARS) {
    gvars[c] = NULL;
    gvars_len[c] = 0;
    gvars_ty[c] = TY_UNKNOWN;
    c++;
  }
}

int defGVar(char* buf, int len, int ty) {
  gvars[gvars_offset] = buf;
  gvars_len[gvars_offset] = len;
  gvars_ty[gvars_offset] = ty;
  gvars_offset++;

  llDefGVar(buf, len, ty, 0);
  return gvars_offset-1;
}
