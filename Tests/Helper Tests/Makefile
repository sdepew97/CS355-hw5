all: helperTest helper clean

helperTest.o: ../../helper.h
	gcc -Wall -c helperTest.c

helper.o: ../../helper.c ../../helper.h
	gcc -Wall -c ../../helper.c

clean:
	rm -rf *.o *.gch *.dSYM