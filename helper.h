//
// Created by Sarah Depew on 3/25/18.
//

#ifndef HW5_HELPER_H
#define HW5_HELPER_H

#include <unistd.h>

#define SUCCESS 0
#define ERROR -1
#define MMAPERROR (void *)-1
#define SIZEOFHEADER 32
#define SIZEOFWORD 8

size_t roundToPage(long currentSize);
size_t roundToWord(long currentSize);

#endif //HW5_HELPER_H
