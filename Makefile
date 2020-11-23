CC = tcc

teal: main.o lex.o parse.o gen.o std.o util.o sys.o
	ld -o $@ $^

main.o: src/main.c
	$(COMPILE.c) $^

lex.o: src/lex.c
	$(COMPILE.c) $^

parse.o: src/parse.c
	$(COMPILE.c) $^

gen.o: src/gen.c
	$(COMPILE.c) $^

std.o: src/std.c
	$(COMPILE.c) $^

util.o: src/util.c
	$(COMPILE.c) $^

sys.o: src/sys.s
	as -o $@ $^

test: teal test/arith.tl test/cmp.tl
	./test.sh test/arith.tl 42
	./test.sh test/cmp.tl 1

fmt: src/main.c src/teal.h src/util.c
	clang-format -i $^

clean:
	$(RM) *.o
	$(RM) teal
