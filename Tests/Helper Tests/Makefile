all: helperTest helper clean

helperTest: helperTest.o helper.o
	gcc -o helperTest helper.o helperTest.o

helperTest.o:
	gcc -Wall -c helperTest.c

helper.o: ../../helper.c ../../helper.h
	gcc -Wall -c ../../helper.c

clean:
	rm -rf *.o *.gch *.dSYM