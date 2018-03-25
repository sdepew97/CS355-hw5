#include <stdio.h>
#include <unistd.h>
//#include "mem.h"

//helper functions
size_t roundToPage(int currentSize) {
    //TODO: implement method that rounds the currentSize to a page size
    int pageSize = getpagesize();
    printf("%d\n", pageSize);
    if((currentSize/pageSize)*pageSize == currentSize) {
        //we are requesting a multiple of page size bytes
        return (size_t) currentSize;
    }

    //we do not have a multiple of the page size, yet, so we must round
    return (size_t) ((((currentSize/pageSize)+1)*pageSize)-pageSize) + pageSize;
}

int main() {
    printf("Hello, World!\n");
    printf("%d\n", roundToPage(4095));
    return 0;
}