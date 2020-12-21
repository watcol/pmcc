#ifndef _PARSE_H_
#define _PARSE_H_

#include"../teal.h"

// void parse();
void panicParse();

// expr.c
int expr();
int expExpr();

// stmt.c
int stmt();
void expStmt();

// func.c
int func();

// type.c
int type();
int expType();

#endif
