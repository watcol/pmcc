CC = tcc

teal: main.o parse.a lex.a gen.a util.o sys.o
	ld -o $@ $^

main.o: src/main.c
	$(COMPILE.c) $^

lex.a: lex.o ch.o eof.o ident.o num.o op.o str.o
	ar r $@ $^

lex.o: src/lex/lex.c
	$(COMPILE.c) $^

ch.o: src/lex/ch.c
	$(COMPILE.c) $^

eof.o: src/lex/eof.c
	$(COMPILE.c) $^

ident.o: src/lex/ident.c
	$(COMPILE.c) $^

num.o: src/lex/num.c
	$(COMPILE.c) $^

op.o: src/lex/op.c
	$(COMPILE.c) $^

str.o: src/lex/str.c
	$(COMPILE.c) $^

parse.a: parse.o expr.o stmt.o func.o type.o
	ar r $@ $^

parse.o: src/parse/parse.c
	$(COMPILE.c) $^

expr.o: src/parse/expr.c
	$(COMPILE.c) $^

stmt.o: src/parse/stmt.c
	$(COMPILE.c) $^

func.o: src/parse/func.c
	$(COMPILE.c) $^

type.o: src/parse/type.c
	$(COMPILE.c) $^

gen.a: gen.o llvm.o
	ar r $@ $^

gen.o: src/gen/gen.c
	$(COMPILE.c) $^

llvm.o: src/gen/llvm.c
	$(COMPILE.c) $^

util.o: src/util.c
	$(COMPILE.c) $^

sys.o: src/sys.s
	as -o $@ $^

test: teal test.sh test/arith.tl test/cmp.tl test/var.tl test/flow.tl
	./test.sh test/arith.tl 42
	./test.sh test/cmp.tl 1
	./test.sh test/var.tl 42
	./test.sh test/flow.tl 2

fmt: src/main.c src/teal.h src/util.c
	clang-format -i $^

clean:
	$(RM) *.o
	$(RM) *.a
	$(RM) teal
