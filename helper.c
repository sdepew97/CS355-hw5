//
// Created by Sarah Depew on 3/31/18.
//
#include "helper.h"

//helper functions
size_t roundToPage(int currentSize) {
    int pageSize = getpagesize();

    if ((currentSize / pageSize) * pageSize == currentSize) {
        //we are requesting a multiple of page size bytes
        return (size_t) currentSize;
    }

    //we do not have a multiple of the page size, yet, so we must round
    return (size_t) ((((currentSize / pageSize) + 1) * pageSize) - pageSize) + pageSize;
}

size_t roundToWord(int currentSize) {
    int wordSize = SIZEOFWORD;

    if ((currentSize / wordSize) * wordSize == currentSize) {
        //we are requesting a multiple of word size bytes
        return (size_t) currentSize;
    }

    //we do not have a multiple of the word size, yet, so we must round
    return (size_t) ((((currentSize / wordSize) + 1) * wordSize) - wordSize) + wordSize;
}