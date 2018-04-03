#include "mem.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>

#define NUM_ALLOC 2000000
#define FREE_FREQ 100          // 1:100
#define COALESCE_FREQ 100000   // 1:100000
#define BYTE 8

clock_t begin, end;

static void print_execution_time(clock_t begin, clock_t end) {
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Execution time: %.2f seconds\n", time_spent);
}

void test_two_mil() {
    int result = Mem_Init(NUM_ALLOC * 96+32);
    assert(result == 0);
    srand(time(NULL));
    void **ptrs = malloc(sizeof(void*) * NUM_ALLOC);

    for (int i = 0; i < NUM_ALLOC; i++) {
        int r = rand()%96+1;
        ptrs[i] = Mem_Alloc(r);
        assert(ptrs[i] != NULL);
    }

    for (int i = 0; i < NUM_ALLOC; i++) {
        // printf("\n%d ",i);
//        if (i % FREE_FREQ == FREE_FREQ - 1)
//            Mem_Free(ptrs[i - FREE_FREQ + 1], i % COALESCE_FREQ == 0);

        Mem_Free(NULL, 1);
    }





    end = clock();
    print_execution_time(begin, end);
    free(ptrs);
}


int main() {
    test_two_mil();
    return EXIT_SUCCESS;
}
