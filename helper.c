//
// Created by Sarah Depew on 3/31/18.
//
#include <unistd.h>
#include "helper.h"

//helper functions
size_t roundToPage(long currentSize) {
    int pageSize = getpagesize();

    if ((currentSize / pageSize) * pageSize == currentSize) {
        //we are requesting a multiple of page size bytes
        return (size_t) currentSize;
    }

    //we do not have a multiple of the page size, yet, so we must round
    return (size_t) ((((currentSize / pageSize) + 1) * pageSize) - pageSize) + pageSize;
}

size_t roundToWord(long currentSize) {
    int wordSize = SIZEOFWORD;

    if ((currentSize / wordSize) * wordSize == currentSize) {
        //we are requesting a multiple of word size bytes
        return (size_t) currentSize;
    }

    //we do not have a multiple of the word size, yet, so we must round
    return (size_t) ((((currentSize / wordSize) + 1) * wordSize) - wordSize) + wordSize;
}