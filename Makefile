teal: main.o util.o
	$(LINK.c) -o $@ $^

main.o: src/main.c
	$(COMPILE.c) $^

util.o: src/util.c
	$(COMPILE.c) $^

clean:
	$(RM) *.o
	$(RM) teal
