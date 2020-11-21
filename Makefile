teal: main.o util.o lex.o
	$(LINK.c) -o $@ $^

main.o: src/main.c
	$(COMPILE.c) $^

util.o: src/util.c
	$(COMPILE.c) $^

lex.o: src/lex.c
	$(COMPILE.c) $^


test: teal test/zero.tl test/number.tl test/addsub.tl
	./test.sh test/zero.tl 0
	./test.sh test/number.tl 42
	./test.sh test/addsub.tl 42

fmt: src/main.c src/teal.h src/util.c
	clang-format -i $^

clean:
	$(RM) *.o
	$(RM) teal
