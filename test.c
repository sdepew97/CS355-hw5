#include <stdio.h>
#include "mem.h"

int main() {
    printf("Hello, World!\n");
    Mem_Init(1);
    int *tryingWrite = Mem_Alloc(sizeof(int));
    Mem_Dump();
    *tryingWrite = 12;
    Mem_Dump();
    printf("value of tryingWrite %d\n", *tryingWrite);
    int *tryingWrite2 = Mem_Alloc(sizeof(int));
    Mem_Dump();
    *tryingWrite2 = 129879807;
    Mem_Dump();
    printf("value of tryingWrite2 %d\n", *tryingWrite2);
    printf("value of memory locations around tryingWrite %d\n", *(tryingWrite2-28));
    return 0;
}