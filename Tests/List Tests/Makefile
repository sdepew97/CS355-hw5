all: cleanFirst list listTestWorstFit listTest cleanLast

helperTest:
	gcc -g -ggdb -o helperTest helperTest.c -L. -lhelper

helper: helper.o
	gcc -g -ggdb -o libhelper.so helper.o -shared

helper.o: ../../helper.c ../../helper.h
	gcc -g -ggdb -Wall -fpic -c ../../helper.c

cleanFirst:
	rm -rf *.o *.gch *.dSYM helperTest

cleanLast:
	rm -rf *.o *.gch *.dSYM