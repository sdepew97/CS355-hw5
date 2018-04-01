//#include "mem.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>
//#include <assert.h>
//#include <limits.h>
//#include <unistd.h>
//
//#define NUM_ALLOC 2000000
//#define FREE_FREQ 100     // 1:100
//#define BYTE 8
//
//clock_t begin, end;
//
//static void print_execution_time(clock_t begin, clock_t end) {
//    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
//    printf("Execution time: %.2f seconds\n", time_spent);
//}
//
//void test_two_mil() {
//    int result = Mem_Init(NUM_ALLOC * 40);
//    assert(result == 0);
//
//    void **ptrs = malloc(sizeof(void*) * NUM_ALLOC);
//    for (int i = 0; i < NUM_ALLOC; i++) {
//        printf("Value of i: %d\n", i);
//        ptrs[i] = Mem_Alloc(BYTE);
//        assert(ptrs[i] != NULL);
//        if (i % FREE_FREQ == FREE_FREQ - 1)
//            Mem_Free(ptrs[i-FREE_FREQ+1], 0);
//    }
//
//    end = clock();
//    print_execution_time(begin, end);
//    free(ptrs);
//}
//
//
//int main() {
//    test_two_mil();
//    return EXIT_SUCCESS;
//}

#include "mem.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>
#include "boolean.h"
#include "helper.h"
//
//#define NUM_ALLOC 2000000
//#define FREE_FREQ 100          // 1:100
//#define COALESCE_FREQ 100000   // 1:100000
//#define BYTE 8
//
//clock_t begin, end;
//
//static void print_execution_time(clock_t begin, clock_t end) {
//    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
//    printf("Execution time: %.2f seconds\n", time_spent);
//}
//
//void test_two_mil() {
//    int result = Mem_Init(NUM_ALLOC * 40);
//    assert(result == 0);
//
//    void **ptrs = malloc(sizeof(void*) * NUM_ALLOC);
//
//    for (int i = 0; i < NUM_ALLOC; i++) {
//        printf("Value of i: %d\n", i);
//        ptrs[i] = Mem_Alloc(BYTE);
//        assert(ptrs[i] != NULL);
//
//        if (i % FREE_FREQ == FREE_FREQ - 1)
//            Mem_Free(ptrs[i-FREE_FREQ+1], i % COALESCE_FREQ == 0);
//
//    }
//    end = clock();
//    print_execution_time(begin, end);
//    free(ptrs);
//}
//
//
//int main() {
//    test_two_mil();
//    return EXIT_SUCCESS;
//}

#define NUM_ALLOC 10

int main() {
    int result = Mem_Init(NUM_ALLOC);
    assert(result == 0);

    if (Mem_Free((void *) 1232343, FALSE) != ERROR) {
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
