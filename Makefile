all: mem test clean

test:
	gcc -g -ggdb -o test test.c -L. -lmem

mem: mem.o helper.o list.o
	gcc -g -ggdb -o libmem.so mem.o -shared

mem.o: mem.c mem.h
	gcc -g -ggdb -Wall -fpic -c mem.c

helper.o: helper.c helper.h
	gcc -g -ggdb -Wall -fpic -c helper.c

list.o: list.c list.h
	gcc -g -ggdb -Wall -fpic -c list.c

clean:
	rm -rf *.o *.gch *.dSYM