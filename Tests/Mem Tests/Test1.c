//
// Created by Sarah Depew on 3/26/18.
//

#include "../../mem.h"
#include "../../helper.h"
#include <stdio.h>
#include <stdlib.h>

#define NUM_ALLOC 7

int test_one() {
    int result = Mem_Init(NUM_ALLOC);
    if(result == ERROR) {
        return EXIT_FAILURE;
    }

    if(Mem_Alloc(NUM_ALLOC) == NULL) {
        return EXIT_FAILURE;
    } else {
        Mem_Dump();
    }

    return EXIT_SUCCESS;
}


int main() {
    return test_one();
}
