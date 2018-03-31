//
// Created by Sarah Depew on 3/31/18.
//

#include "../../helper.h"
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Round up to page result for odd: %ld\n", roundToPage(1));
    printf("Round up to page result for aligned: %ld\n", roundToPage(getpagesize()));

    printf("Round up to word result for odd: %ld\n", roundToWord(13));
    printf("Round up to word result for aligned: %ld\n", roundToWord(8));
    printf("Round up to word result for aligned: %ld\n", roundToWord(64));

    return 0;
}