#include <stdio.h>
#include "mem.h"

int main() {
    printf("Hello, World!\n");
    Mem_Init(1);
    Mem_Alloc(4064);
    Mem_Dump();
    return 0;
}