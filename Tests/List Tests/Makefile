all: cleanFirst list listTestAddHeaderFree listTestAddHeaderMain listTestCheckValidPtrMain listTestCoalesceFreeList listTestFindPreviousFree listTestLocalCoalesceFree listTestRemoveHeaderFree listTestRemoveHeaderMain listTestSortFreeList listTestSortList listTestWorstFit cleanLast

listTestAddHeaderFree:
	gcc -g -ggdb -o listTestAddHeaderFree listTestAddHeaderFree.c -L. -llist

listTestAddHeaderMain:
	gcc -g -ggdb -o listTestAddHeaderMain listTestAddHeaderMain.c -L. -llist

listTestCheckValidPtrMain:
	gcc -g -ggdb -o listTestCheckValidPtrMain listTestCheckValidPtrMain.c -L. -llist

listTestCoalesceFreeList:
	gcc -g -ggdb -o listTestCoalesceFreeList listTestCoalesceFreeList.c -L. -llist

listTestFindPreviousFree:
	gcc -g -ggdb -o listTestFindPreviousFree listTestFindPreviousFree.c -L. -llist

listTestLocalCoalesceFree:
	gcc -g -ggdb -o listTestLocalCoalesceFree listTestLocalCoalesceFree.c -L. -llist

listTestRemoveHeaderFree:
	gcc -g -ggdb -o listTestRemoveHeaderFree listTestRemoveHeaderFree.c -L. -llist

listTestRemoveHeaderMain:
	gcc -g -ggdb -o listTestRemoveHeaderMain listTestRemoveHeaderMain.c -L. -llist

listTestSortFreeList:
	gcc -g -ggdb -o listTestSortFreeList listTestSortFreeList.c -L. -llist

listTestSortList:
	gcc -g -ggdb -o listTestSortList listTestSortList.c -L. -llist

listTestWorstFit:
	gcc -g -ggdb -o listTestWorstFit listTestWorstFit.c -L. -llist

list: list.o helper.o
	gcc -g -ggdb -o liblist.so list.o helper.o -shared

list.o: ../../list.c ../../list.h
	gcc -g -ggdb -Wall -fpic -c ../../list.c

helper.o: ../../helper.c ../../helper.h
	gcc -g -ggdb -Wall -fpic -c ../../helper.c

cleanFirst:
	rm -rf *.o *.gch *.dSYM listTestAddHeaderFree listTestAddHeaderMain listTestCheckValidPtrMain listTestCoalesceFreeList listTestFindPreviousFree listTestLocalCoalesceFree listTestRemoveHeaderFree listTestRemoveHeaderMain listTestSortFreeList listTestSortList listTestWorstFit

cleanLast:
	rm -rf *.o *.gch *.dSYM