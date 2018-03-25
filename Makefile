all: mem test clean

test:
	gcc -g -ggdb -o test test.c -L. -lmem

mem: mem.o
	gcc -g -ggdb -o libmem.so mem.o -shared

mem.o: mem.c mem.h
	gcc -g -ggdb -Wall -fpic -c mem.c

clean:
	rm -rf *.o *.gch *.dSYM