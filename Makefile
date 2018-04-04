all: cleanFirst mem test cleanLast

test:
	gcc -g -ggdb -o test test.c -L. -lmem

worstcase: helper.o list.o
	gcc -g -DWC -ggdb -Wall -fpic -c mem.c
	gcc -g -ggdb -o libmem.so mem.o helper.o list.o -shared

averagecase: helper.o list.o
	gcc -g -DAC -ggdb -Wall -fpic -c mem.c
	gcc -g -ggdb -o libmem.so mem.o helper.o list.o -shared

alignedcase: helper.o list.o
	gcc -g -DAL -ggdb -Wall -fpic -c mem.c
	gcc -g -ggdb -o libmem.so mem.o helper.o list.o -shared

mem: helper.o list.o
	gcc -g -DM -ggdb -Wall -fpic -c mem.c
	gcc -g -ggdb -o libmem.so mem.o helper.o list.o -shared

helper.o: helper.c helper.h
	gcc -g -ggdb -Wall -fpic -c helper.c

list.o: list.c list.h
	gcc -g -ggdb -Wall -fpic -c list.c

cleanFirst:
	rm -rf *.o *.gch *.dSYM test

cleanLast:
	rm -rf *.o *.gch *.dSYM