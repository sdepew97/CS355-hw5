//
// Created by Sarah Depew on 3/31/18.
//

#include "../../helper.h"
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Round up to page result for odd: %d, %ld\n", 1, roundToPage(1));
    printf("Round up to page result for aligned: %d, %ld\n", getpagesize(), roundToPage(getpagesize()));

    printf("Round up to word result for odd: %d, %ld\n", 13, roundToWord(13));
    printf("Round up to word result for aligned: %d, %ld\n", 8, roundToWord(8));
    printf("Round up to word result for aligned: %d, %ld\n", 64, roundToWord(64));

    return 0;
}