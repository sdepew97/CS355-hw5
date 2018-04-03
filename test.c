#include "mem.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>



int main() {
    if(Mem_Init(-10) < 0)
        printf("Sad, I failed\n");
    else
        printf("init successfully\n");
    printf("The mem_init should fail and we will call Mem_Alloc Mem_Free and Mem_Dump, none of them should work\n");
    Mem_Dump();
    void* test = Mem_Alloc(8);
    assert(test == NULL);
    assert(Mem_Free(test,1) == -1);

    return 0;
}