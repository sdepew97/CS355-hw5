#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "mem.h"


int main() {
    printf("Size of char %ld\n", sizeof(char));
    printf("Size of long %ld\n", sizeof(long));
    printf("Size of int %ld\n", sizeof(int));
    printf("Size of header %ld\n", sizeof(header));

    printf("Hello, World!\n");
    Mem_Init(1000);

    if(Mem_Init != 0) {
        exit(EXIT_FAILURE);
    }

    for(int i=0; i< 10000; i++) {
        int *tryingWrite = Mem_Alloc(sizeof(int));
        if(tryingWrite == NULL) {
            break;
        }
//    Mem_Dump();
        *tryingWrite = 12;
//    Mem_Dump();
        printf("value of tryingWrite %d\n", *tryingWrite);
    }
//    int *tryingWrite2 = Mem_Alloc(sizeof(int));
//    Mem_Dump();
//    *tryingWrite2 = 129879807;
//    Mem_Dump();
//    printf("value of tryingWrite2 %d\n", *tryingWrite2);
//    printf("value of memory locations around tryingWrite %d\n", *(tryingWrite2-28));
    return 0;
}