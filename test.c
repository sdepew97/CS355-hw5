#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "boolean.h"
#include "mem.h"


int main() {
    printf("Size of char %ld\n", sizeof(char));
    printf("Size of long %ld\n", sizeof(long));
    printf("Size of int %ld\n", sizeof(int));
    printf("Size of header %ld\n", sizeof(header));

    printf("Hello, World!\n");

    Mem_Init(16);
    Mem_Dump();

//    if(Mem_Init(10000) != 0) {
//        exit(EXIT_FAILURE);
//    }

//    for(int i=0; i< 10000; i++) {
//        int *tryingWrite = Mem_Alloc(sizeof(int));
//        if(tryingWrite == NULL) {
//            break;
//        }
////    Mem_Dump();
//        *tryingWrite = 12;
////    Mem_Dump();
//        printf("value of tryingWrite %d\n", *tryingWrite);
//    }

    void *allocated = Mem_Alloc(1);
    void *allocated2 = Mem_Alloc(7);
    void *allocated3 = Mem_Alloc(8);
    Mem_Dump();
//    Mem_Free(allocated, FALSE);
    Mem_Free(allocated, TRUE);
    Mem_Free(allocated2, TRUE);
    Mem_Dump();
    Mem_Free(allocated2, FALSE);
    Mem_Free(allocated3, FALSE);
    Mem_Dump();

//    *tryingWrite2 = 129879807;
//    Mem_Dump();
//    printf("value of tryingWrite2 %d\n", *tryingWrite2);
//    printf("value of memory locations around tryingWrite %d\n", *(tryingWrite2-28));
    return 0;
}