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
//    void **ptrs = malloc(sizeof(void *) * NUM_ALLOC);
//    for (int i = 0; i < NUM_ALLOC; i++) {
////        printf("Value of i: %d\n", i);
//        ptrs[i] = Mem_Alloc(BYTE);
//        assert(ptrs[i] != NULL);
//        if (i % FREE_FREQ == FREE_FREQ - 1)
////            Mem_Free(ptrs[i - FREE_FREQ + 1], 1);
//            Mem_Free(ptrs[i - FREE_FREQ + 1], i % COALESCE_FREQ == 0);
//    }
//
//    end = clock();
//    print_execution_time(begin, end);
//    free(ptrs);
//}
//
//int main() {
//    test_two_mil();
//    return EXIT_SUCCESS;
//}

//#include "mem.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>
//#include <assert.h>
//#include <limits.h>
//#include <unistd.h>
//#include "boolean.h"
//#include "list.h"
//#include "helper.h"
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
//    void **ptrs = malloc(sizeof(void *) * NUM_ALLOC);
//
//    for (int i = 0; i < NUM_ALLOC; i++) {
//        printf("Value of i: %d\n", i);
//        ptrs[i] = Mem_Alloc(BYTE);
//        assert(ptrs[i] != NULL);
//
//        if (i % FREE_FREQ == FREE_FREQ - 1)
//            Mem_Free(ptrs[i - FREE_FREQ + 1], i % COALESCE_FREQ == 0);
//    }
//
////    for (int i = 0; i < NUM_ALLOC; i++) {
////        printf("Iteration %d\n", i);
////        if (Mem_Free(ptrs[i], 1) == ERROR) {
////            printf("got here\n");
////            break;
////        }
////    }
//
//    end = clock();
//    print_execution_time(begin, end);
////    Mem_Dump();
//    free(ptrs);
//}
//
//
//int main() {
////    printf("sizeof header %ld\n", sizeof(header));
////    printf("sizeof void * %ld\n", sizeof(void *));
//    test_two_mil();
//    return EXIT_SUCCESS;
//}


#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include "mem.h"
#define TRUE 1
#define FALSE 0
#define PAGE 4096

// helper function to free the arrays of pointers we make using malloc
void free_ptrs(void **ptrs) {
    for (int i = 0; ptrs[i] != NULL; i++) {
        free(ptrs[i]);
    }
}

// 1. check for 8-byte alignment
int eight_byte() {
    void **ptrs = malloc(sizeof(void *) *30);
    for (int i = 0; i < 30; i++) {
        ptrs[i] = Mem_Alloc(7);
        if (ptrs[i] == NULL) {
            return FALSE;
        }
        if (((long) ptrs[i] % 8 != 0)) {
            return FALSE;
        }
    }
    free_ptrs(ptrs);
    return TRUE;
}

// 2. allocate exactly 8 bytes
int simple_eight_byte() {
    void **ptrs = malloc(sizeof(void *) *30);
    for (int i = 0; i < 30; i++) {
        ptrs[i] = Mem_Alloc(8);
        if (ptrs[i] == NULL) {
            return FALSE;
        }
        if (((long) ptrs[i] % 8 != 0)) {
            return FALSE;
        }
    }
    free_ptrs(ptrs);
    return TRUE;
}

// 3. make multiple allocations of size that's a multiple of 8
int aligned_allocations() {
    void **ptrs = malloc(sizeof(void *) *30);
    int j = 8;
    for (int i = 0; i < 30; i++) {
        ptrs[i] = Mem_Alloc(j);
        if (ptrs[i] == NULL) {
            return FALSE;
        }
        if (((long) ptrs[i] % 8 != 0)) {
            return FALSE;
        }
        j+=j;
    }
    free_ptrs(ptrs);
    return TRUE;
}

// 4. make multiple allocations of odd number size
int odd_allocations() {
    void **ptrs = malloc(sizeof(void *) *30);
    int j = 1;
    for (int i = 0; i < 30; i++) {
        ptrs[i] = Mem_Alloc(j);
        if (ptrs[i] == NULL) {
            return FALSE;
        }
        if (((long) ptrs[i] % 8 != 0)) {
            return FALSE;
        }
        j+=2;
    }
    free_ptrs(ptrs);
    return TRUE;
}

// 5. Pass invalid arguments to Mem_Init()
int invalid_mem_init() {
    int ret = Mem_Init(-1);
    if (ret == -1) {
        // we get an error, what we expect with a -1 passed in
        free_ptrs(ptrs);
        return TRUE;
    } else {
        return FALSE;
    }
}


int main() {
    if (Mem_Init(PAGE) != 0) {
        return EXIT_FAILURE;
    } else {
        printf("Test 1: 8 byte alignment\n");
        printf("RESULT: %s\n", eight_byte() == TRUE ? "PASSED" : "FAILED");
        printf("-------------------------\n");
        printf("Test 2: Simple 8 byte allocation\n");
        printf("RESULT: %s\n", simple_eight_byte() == TRUE ? "PASSED" : "FAILED");
        printf("-------------------------\n");
        printf("Test 3: Multiple aligned allocations\n");
        printf("RESULT: %s\n", aligned_allocations() == TRUE ? "PASSED" : "FAILED");
        printf("-------------------------\n");
        printf("Test 4: Multiple odd number size allocations\n");
        printf("RESULT: %s\n", odd_allocations() == TRUE ? "PASSED" : "FAILED");
        printf("-------------------------\n");
        printf("Test 5: Invalid Mem_Init()\n");
        printf("RESULT: %s\n", invalid_mem_init() == TRUE ? "PASSED" : "FAILED");
    }
}
