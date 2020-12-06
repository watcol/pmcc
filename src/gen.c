#include"teal.h"
#define MAX_VARS 100

int lvars_offset;
char* lvars[MAX_VARS];
int lvars_ty[MAX_VARS];

void cleanVar() {
  lvars_offset = 0;

  int c = 0;
  while(c < MAX_VARS) {
    lvars[c] = NULL;
    lvars_ty[c] = TY_UNKNOWN;
    c++;
  }
}

void initCode() {
  cleanVar();
}

void funcBegin(char* name, int ret, int* args, int argc) {
  llFuncBegin(name, ret, args, argc);

  lvars_offset = argc;
  int c = 0;
  while(c < argc) {
    lvars_ty[c] = args[c];
    c++;
  }
}

void funcEnd() {
  cleanVar();
  llFuncEnd();
}

int lVarFind(char* cur, int len) {
  int c = 0;
  while(!strCmp(lvars[c], cur, len) && c < lvars_offset) c++;

  if(c == lvars_offset) return -1;

  return c;
}

int lVarAdd(char* buf, int ty) {
  if(lvars_offset == MAX_VARS) panic("Too many variables.");

  int id = lvars_offset;

  lvars_offset++;
  lvars[id] = buf;
  lvars_ty[id] = ty;

  return id;
}

int lVarType(int id) {
  return lvars_ty[id];
}

int lVarGet(char *cur, int len, int ty) {
  int c = lVarFind(cur, len);

  if(c == -1) c = lVarAdd(cur, ty);

  if(lVarType(c) != ty) panic("Type unmatched");
  return c;
}

void pushl() {
  panic("Unimplemented");
}

void pushm(int val) {
  panic("Unimplemented");
}

void pushn(int val) {
  panic("Unimplemented");
}

void inc(int mem) {
  panic("Unimplemented");
}

void dec(int mem) {
  panic("Unimplemented");
}

void add(int type) {
  panic("Unimplemented");
}

void sub(int type) {
  panic("Unimplemented");
}

void mul(int type) {
  panic("Unimplemented");
}

void div(int type) {
  panic("Unimplemented");
}

void rem(int type) {
  panic("Unimplemented");
}

void not_(int type) {
  panic("Unimplemented");
}

void less(int type) {
  panic("Unimplemented");
}

void leq(int type) {
  panic("Unimplemented");
}

void more(int type) {
  panic("Unimplemented");
}

void meq(int type) {
  panic("Unimplemented");
}

void eq(int type) {
  panic("Unimplemented");
}

void neq(int type) {
  panic("Unimplemented");
}

void and_() {
  panic("Unimplemented");
}

void or_() {
  panic("Unimplemented");
}

void asg(int type) {
  panic("Unimplemented");
}

void addasg(int type) {
  panic("Unimplemented");
}

void subasg(int type) {
  panic("Unimplemented");
}

void mulasg(int type) {
  panic("Unimplemented");
}

void divasg(int type) {
  panic("Unimplemented");
}

void remasg(int type) {
  panic("Unimplemented");
}

void ret(int ty, int val) {
  panic("Unimplemented");
}

int ifBegin() {
  panic("Unimplemented");
  return 0;
}

void ifElse(int id) {
  panic("Unimplemented");
}

void ifEnd(int id) {
  panic("Unimplemented");
}

int whileBegin() {
  panic("Unimplemented");
  return 0;
}

void whileEval(int id) {
  panic("Unimplemented");
}

void whileEnd(int id) {
  panic("Unimplemented");
}

