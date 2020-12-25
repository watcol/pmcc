CC = tcc

all:
	make clean
	make main

main: main.o parse.a lex.a gen.a util.o sys.o
	ld -o $@ $^

main.o: ../src/main.c
	$(COMPILE.c) $^

lex.a: lex.o basic.o eof.o ident.o num.o op.o str.o
	ar r $@ $^

lex.o: ../src/lex/lex.c
	$(COMPILE.c) $^

basic.o: ../src/lex/basic.c
	$(COMPILE.c) $^

eof.o: ../src/lex/eof.c
	$(COMPILE.c) $^

ident.o: ../src/lex/ident.c
	$(COMPILE.c) $^

num.o: ../src/lex/num.c
	$(COMPILE.c) $^

op.o: ../src/lex/op.c
	$(COMPILE.c) $^

str.o: ../src/lex/str.c
	$(COMPILE.c) $^

parse.a: parse.o expr.o stmt.o func.o type.o
	ar r $@ $^

parse.o: ../src/parse/parse.c
	$(COMPILE.c) $^

expr.o: ../src/parse/expr.c
	$(COMPILE.c) $^

stmt.o: ../src/parse/stmt.c
	$(COMPILE.c) $^

func.o: ../src/parse/func.c
	$(COMPILE.c) $^

type.o: ../src/parse/type.c
	$(COMPILE.c) $^

gen.a: gen.o flow.o funcs.o gvars.o ops.o llvm.o vars.o
	ar r $@ $^

gen.o: ../src/gen/gen.c
	$(COMPILE.c) $^

flow.o: ../src/gen/flow.c
	$(COMPILE.c) $^

funcs.o: ../src/gen/funcs.c
	$(COMPILE.c) $^

gvars.o: ../src/gen/gvars.c
	$(COMPILE.c) $^

ops.o: ../src/gen/ops.c
	$(COMPILE.c) $^

llvm.o: ../src/gen/llvm.c
	$(COMPILE.c) $^

vars.o: ../src/gen/vars.c
	$(COMPILE.c) $^

util.o: ../src/util.c
	$(COMPILE.c) $^

sys.o: ../src/sys.s
	as -o $@ $^

test: main
	./test.sh

clean:
	$(RM) *.o
	$(RM) *.a
	$(RM) main

.PHONY: all test clean
