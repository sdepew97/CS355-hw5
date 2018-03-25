#include <stdio.h>
#include "mem.h"

int main() {
    printf("Hello, World!\n");
    printf("%d\n", roundToPage(4000));
    Mem_Init(4096);
    return 0;
}