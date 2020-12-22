#include"gen.h"

int bbs_offset;

void cleanBb() {
  bbs_offset = 0;
}

int ifBegin(int var) {
  bbs_offset++;
  int id = bbs_offset;

  int dvar = derefVar(var);
  int cond = lTmpVar(TY_I1);
  llIcmpNVAsg("ne", cond, 0, dvar);
  llBrCond(cond, id, BB_IF_BEGIN, BB_IF_ELSE);
  llBb(id, BB_IF_BEGIN);

  return id;
}

void ifElse(int id) {
  llBr(id, BB_IF_END);
  llBb(id, BB_IF_ELSE);
}

void ifEnd(int id) {
  llBr(id, BB_IF_END);
  llBb(id, BB_IF_END);
}

int whileBegin() {
  bbs_offset++;
  int id = bbs_offset;
  llBr(id, BB_WHILE_EVAL);
  llBb(id, BB_WHILE_EVAL);
  return id;
}

void whileEval(int id, int var) {
  int dvar = derefVar(var);
  int cond = lTmpVar(TY_I1);
  llIcmpNVAsg("ne", cond, 0, dvar);
  llBrCond(cond, id, BB_WHILE_BEGIN, BB_WHILE_END);

  llBb(id, BB_WHILE_BEGIN);
}

void whileEnd(int id) {
  llBr(id, BB_WHILE_EVAL);
  llBb(id, BB_WHILE_END);
}

