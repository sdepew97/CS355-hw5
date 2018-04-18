#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "mem.h"

//int main() {
//    int m_error = -1;
//
//    //put test here
//    printf("%d\n", Mem_Init(-10));
//    printf("%d\n", m_error);
//    return 0;
//}

int main() {
//    assert(Mem_Init(-4096) == -1);
//    assert(m_error == E_BAD_ARGS);
//    exit(0);

//    assert(Mem_Init(4096) == 0);
//    assert(Mem_Alloc(2048) != NULL);
//
//    assert(Mem_Alloc(2049) == NULL);
//    printf("%d\n", m_error);
//    assert(m_error == E_NO_SPACE);

    assert(Mem_Init(4096) == 0);
    void *ptr[4];

    ptr[0] = Mem_Alloc(900);
    assert(ptr[0] != NULL);

    ptr[1] = Mem_Alloc(900);
    assert(ptr[1] != NULL);

    ptr[2] = Mem_Alloc(900);
    assert(ptr[2] != NULL);

    assert(Mem_Free(ptr[1], 0) == 0);
    ptr[1] = NULL;

    ptr[3] = Mem_Alloc(1100);
    assert()

    exit(0);
}