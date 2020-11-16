teal: main.o
	$(LINK.c) -o $@ $^

main.o: src/main.c
	$(COMPILE.c) $^

clean:
	$(RM) *.o
	$(RM) teal
