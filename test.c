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
    return 0;
}