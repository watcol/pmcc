CC = tcc

teal: main.o lex.o parse.a gen_llvm.o gen.o util.o sys.o
	ld -o $@ $^

main.o: src/main.c
	$(COMPILE.c) $^

lex.o: src/lex.c
	$(COMPILE.c) $^

parse.a: parse.o parse_expr.o parse_stmt.o parse_func.o
	ar r $@ $^

parse.o: src/parse/parse.c
	$(COMPILE.c) $^

parse_expr.o: src/parse/expr.c
	$(COMPILE.c) $^

parse_stmt.o: src/parse/stmt.c
	$(COMPILE.c) $^

parse_func.o: src/parse/func.c
	$(COMPILE.c) $^

gen.o: src/gen.c
	$(COMPILE.c) $^

gen_llvm.o: src/gen_llvm.c
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
	$(RM) teal
