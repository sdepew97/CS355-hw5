#include "../../mem.h"
#include "../../boolean.h"
#include "../../helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>

#define NUM_ALLOC 10

int main() {
    int result = Mem_Init(NUM_ALLOC);
    assert(result == 0);

    if (Mem_Free((void *) 1232343, FALSE) != ERROR) {
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}