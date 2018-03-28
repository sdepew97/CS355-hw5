//
// Created by Sarah Depew on 3/25/18.
//

#ifndef HW5_LIST_H
#define HW5_LIST_H

typedef struct header {
//    boolean free;
    //look up type for single bit for free or not... //TODO
    //TODO: pad to 32 bytes here and then set check sums
    char firstCheckSum;

    struct header *nextHeader; //first linked list
    long amountAllocated; //to be used for compaction
    struct header *nextFree; //second linked list
    char free;
    //TODO: figure out how to add another check sum here...
} header;

extern header *headMainList;
extern header *headFreeList;

header *worstFit(header *head);
void coalesceList(header *head);

//go through free list

//combine neighboring memory sections

#endif //HW5_LIST_H
