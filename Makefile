teal: main.o util.o
	$(LINK.c) -o $@ $^

main.o: src/main.c
	$(COMPILE.c) $^

util.o: src/util.c
	$(COMPILE.c) $^

test: teal test/zero.tl
	./test.sh test/zero.tl 0
	./test.sh test/number.tl 42

clean:
	$(RM) *.o
	$(RM) teal