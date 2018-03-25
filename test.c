#include <stdio.h>
#include "mem.h"

int main() {
    printf("Hello, World!\n");
    printf("%d\n", roundToPage(4098));
    Mem_Init(4096);
    return 0;
}