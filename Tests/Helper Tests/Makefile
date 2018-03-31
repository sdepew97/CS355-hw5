all: helperTest helper clean

helperTest:
	gcc -g -ggdb -o helperTest helperTest.c -L. -lhelper

helper: helper.o
	gcc -g -ggdb -o libhelper.so helper.o -shared

helper.o: ../../helper.c ../../helper.h
	gcc -g -ggdb -Wall -fpic -c ../../helper.c

clean:
	rm -rf *.o *.gch *.dSYM