#include <stdio.h>
#include "mem.h"

int main() {
    printf("Hello, World!\n");
    Mem_Init(1);
    int *tryingWrite = Mem_Alloc(8);

    Mem_Dump();
    return 0;
}