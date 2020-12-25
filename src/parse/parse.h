#ifndef _PARSE_H_
#define _PARSE_H_

#include"../pmcc.h"

// void parse();
void panicParse(char* at);

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
