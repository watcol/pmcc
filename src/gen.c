#include"teal.h"
#define MAX_VARS 100

int lvars_offset;
char* lvars[MAX_VARS];
int lvars_ty[MAX_VARS];

void clean_var() {
  lvars_offset = 0;

  int c = 0;
  while(c < MAX_VARS) {
    lvars[c] = NULL;
    lvars_ty[c] = TY_UNKNOWN;
    c++;
  }
}

void init_code() {
  clean_var();
}

void func_begin(char* name, int ret, int* args, int argc) {
  llfunc_begin(name, ret, args, argc);

  lvars_offset = argc;
  int c = 0;
  while(c < argc) {
    lvars_ty[c] = args[c];
    c++;
  }
}

void func_end() {
  clean_var();
  llfunc_end();
}

int lvar_find(char* cur, int len) {
  int c = 0;
  while(!str_cmp(lvars[c], cur, len) && c < lvars_offset) c++;

  if(c == lvars_offset) return -1;

  return c;
}

int lvar_add(char* buf, int ty) {
  if(lvars_offset == MAX_VARS) panic("Too many variables.");

  int id = lvars_offset;

  lvars_offset++;
  lvars[id] = buf;
  lvars_ty[id] = ty;

  return id;
}

int lvar_type(int id) {
  return lvars_ty[id];
}

int lvar_get(char *cur, int len, int ty) {
  int c = lvar_find(cur, len);

  if(c == -1) c = lvar_add(cur, ty);

  if(lvar_type(c) != ty) panic("Type unmatched");
  return c;
}

void pushl() {
  eputs("Unimplemented");
  sys_exit(1);
}

void pushm(int val) {
  eputs("Unimplemented");
  sys_exit(1);
}

void pushn(int val) {
  eputs("Unimplemented");
  sys_exit(1);
}

void inc(int mem) {
  eputs("Unimplemented");
  sys_exit(1);
}

void dec(int mem) {
  eputs("Unimplemented");
  sys_exit(1);
}

void add(int type) {
  eputs("Unimplemented");
  sys_exit(1);
}

void sub(int type) {
  eputs("Unimplemented");
  sys_exit(1);
}

void mul(int type) {
  eputs("Unimplemented");
  sys_exit(1);
}

void div(int type) {
  eputs("Unimplemented");
  sys_exit(1);
}

void rem(int type) {
  eputs("Unimplemented");
  sys_exit(1);
}

void not_(int type) {
  eputs("Unimplemented");
  sys_exit(1);
}

void less(int type) {
  eputs("Unimplemented");
  sys_exit(1);
}

void leq(int type) {
  eputs("Unimplemented");
  sys_exit(1);
}

void more(int type) {
  eputs("Unimplemented");
  sys_exit(1);
}

void meq(int type) {
  eputs("Unimplemented");
  sys_exit(1);
}

void eq(int type) {
  eputs("Unimplemented");
  sys_exit(1);
}

void neq(int type) {
  eputs("Unimplemented");
  sys_exit(1);
}

void and_() {
  eputs("Unimplemented");
  sys_exit(1);
}

void or_() {
  eputs("Unimplemented");
  sys_exit(1);
}

void asg(int type) {
  eputs("Unimplemented");
  sys_exit(1);
}

void addasg(int type) {
  eputs("Unimplemented");
  sys_exit(1);
}

void subasg(int type) {
  eputs("Unimplemented");
  sys_exit(1);
}

void mulasg(int type) {
  eputs("Unimplemented");
  sys_exit(1);
}

void divasg(int type) {
  eputs("Unimplemented");
  sys_exit(1);
}

void remasg(int type) {
  eputs("Unimplemented");
  sys_exit(1);
}

void ret(int ty, int val) {
  llretn(ty, val);
}

int if_begin() {
  eputs("Unimplemented");
  sys_exit(1);
  return 0;
}

void if_else(int id) {
  eputs("Unimplemented");
  sys_exit(1);
}

void if_end(int id) {
  eputs("Unimplemented");
  sys_exit(1);
}

int while_begin() {
  eputs("Unimplemented");
  sys_exit(1);
  return 0;
}

void while_eval(int id) {
  eputs("Unimplemented");
  sys_exit(1);
}

void while_end(int id) {
  eputs("Unimplemented");
  sys_exit(1);
}

