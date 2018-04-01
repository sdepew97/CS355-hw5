#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "boolean.h"
#include "helper.h"
#include "mem.h"


int main() {
    printf("Size of char %ld\n", sizeof(char));
    printf("Size of long %ld\n", sizeof(long));
    printf("Size of int %ld\n", sizeof(int));
    printf("Size of header %ld\n", sizeof(header));

    printf("Hello, World!\n");

    if (Mem_Init(16) == ERROR) {
        exit(EXIT_FAILURE);
    }

    Mem_Dump();

    void *allocated = Mem_Alloc(1);
    if (allocated == NULL) {
        exit(EXIT_FAILURE);
    }

    void *allocated2 = Mem_Alloc(7);
    if (allocated2 == NULL) {
        exit(EXIT_FAILURE);
    }

    void *allocated3 = Mem_Alloc(8);
    if (allocated3 == NULL) {
        exit(EXIT_FAILURE);
    }

    Mem_Dump();
//    Mem_Free(allocated, FALSE);
    if (Mem_Free(allocated2, FALSE) == ERROR) {
        exit(EXIT_FAILURE);
    }
    printf("after second freed\n");
    Mem_Dump();

    if (Mem_Free(allocated, FALSE) == ERROR) {
        exit(EXIT_FAILURE);
    }
    printf("after first freed\n");
    Mem_Dump();

    if (Mem_Free(allocated3, TRUE) == ERROR) {
        exit(EXIT_FAILURE);
    }
    printf("after third freed\n");
    Mem_Dump();

    exit(EXIT_SUCCESS);
}